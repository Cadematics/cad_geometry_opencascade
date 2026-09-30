# 02 — Incidence and Adjacency

This exercise builds explicit incidence and adjacency relationships between the topological entities of a B-rep model.

The goal is to move from simply traversing a CAD model to representing its topology as structured data.

---

## Objective

Given a STEP file, extract the B-rep topology and construct the following relationships:

```text
Face  → Edge
Edge  → Face

Edge  → Vertex
Vertex → Edge

Face  ↔ Face
```

The final face-to-face adjacency graph is derived from the edge-to-face incidence relationship.

This is a foundational step toward representing CAD geometry as a graph.

---

## B-rep topology

A Boundary Representation (B-rep) describes a solid using topological entities together with their underlying geometry.

The basic hierarchy is:

```text
Solid
 └── Shell
      └── Face
           └── Edge
                └── Vertex
```

However, this hierarchy alone is not sufficient to describe all topological relationships.

For example, an edge can belong to two faces, and two faces can share an edge.

Therefore, we explicitly construct incidence relationships.

---

## Incidence vs adjacency

### Incidence

Incidence describes a relationship between different types of topological entities.

Examples:

```text
Face → Edge
Edge → Face
Edge → Vertex
Vertex → Edge
```

For example:

```text
Face 1 → E1 E2 E3 E4
```

means that Face 1 is bounded by those four edge occurrences.

Similarly:

```text
Edge 1 → F1 F2
```

means that Edge 1 is incident to Faces 1 and 2.

---

### Adjacency

Adjacency describes a relationship between entities of the same type.

Here we derive:

```text
Face ↔ Face
```

Two faces are adjacent when they share an edge.

For example:

```text
Face 1 <-> Face 2 via Edge 1
```

means that Faces 1 and 2 meet along Edge 1.

If two faces share multiple edges, all shared edges are retained:

```text
Face 1 <-> Face 10 via Edge 1 Edge 3
```

This preserves the actual topological relationship instead of reducing it to a simple Boolean connection.

---

# Indexed topology

OCCT provides:

```cpp
TopTools_IndexedMapOfShape
```

which allows us to assign stable integer IDs to unique topological shapes.

We construct three maps:

```cpp
TopTools_IndexedMapOfShape faceMap;
TopTools_IndexedMapOfShape edgeMap;
TopTools_IndexedMapOfShape vertexMap;
```

and populate them with:

```cpp
TopExp::MapShapes(shape, TopAbs_FACE, faceMap);
TopExp::MapShapes(shape, TopAbs_EDGE, edgeMap);
TopExp::MapShapes(shape, TopAbs_VERTEX, vertexMap);
```

The resulting IDs are used throughout the program:

```text
Face 1
Face 2
...

Edge 1
Edge 2
...

Vertex 1
Vertex 2
...
```

This is preferable to identifying topology using geometric coordinates.

Two distinct topological vertices can have the same geometric location, and the same geometric entity can appear in different topological contexts.

For topology analysis, the topological identity is what matters.

---

# Face → Edge

For every face, we traverse its edges:

```cpp
for (TopExp_Explorer explorer(face, TopAbs_EDGE);
     explorer.More();
     explorer.Next())
{
    TopoDS_Shape edge = explorer.Current();
}
```

The edge is converted into its indexed ID using:

```cpp
int edgeId = edgeMap.FindIndex(edge);
```

Example:

```text
Face 1: E1 E2 E3 E4
```

This gives the boundary-edge representation of each face.

---

# Edge → Vertex

An edge is bounded by vertices.

We therefore traverse:

```text
Edge → Vertex
```

using `TopExp_Explorer`.

Example:

```text
Edge 1: V1 V2
Edge 2: V1 V3
Edge 3: V3 V4
```

An important observation is that an edge does not necessarily produce two different vertex IDs.

For example, in `cube_hole.step`:

```text
Edge 10: V8 V8
Edge 14: V10 V10
```

These are seam edges associated with the cylindrical hole.

Therefore, code should not blindly assume:

```text
edge → exactly two distinct vertices
```

Topology can contain special cases such as seam edges and degenerate edges.

---

# Edge → Face

OCCT provides:

```cpp
TopExp::MapShapesAndAncestors()
```

which is useful for constructing incidence relationships.

We use:

```cpp
TopExp::MapShapesAndAncestors(
    shape,
    TopAbs_EDGE,
    TopAbs_FACE,
    edgeFaceMap
);
```

This produces:

```text
Edge → Faces
```

For example:

```text
Edge 1: F1 F2
Edge 2: F1 F5
Edge 3: F1 F4
```

This relationship is particularly useful because it allows us to derive face adjacency without performing geometric intersection tests.

---

# Vertex → Edge

The reverse relationship is also constructed:

```cpp
TopExp::MapShapesAndAncestors(
    shape,
    TopAbs_VERTEX,
    TopAbs_EDGE,
    vertexEdgeMap
);
```

This produces:

```text
Vertex → Edges
```

For example:

```text
Vertex 1: E1 E2 E5
```

This completes the four basic incidence relationships:

```text
Face   → Edge
Edge   → Face

Edge   → Vertex
Vertex → Edge
```

Together, these relationships form the basic connectivity structure of the B-rep.

---

# Face → Face adjacency

Face adjacency is not extracted independently.

Instead, it is derived from:

```text
Edge → Face
```

For every edge:

1. Find its incident faces.
2. Remove duplicate face IDs.
3. Generate pairs of distinct faces.
4. Store the edge that connects the pair.

Conceptually:

```text
Edge E
   │
   ├── Face A
   └── Face B

        ↓

Face A ↔ Face B
```

The implementation stores:

```cpp
std::map<
    std::pair<int, int>,
    std::vector<int>
> faceAdjacency;
```

The key is the pair:

```text
(Face A, Face B)
```

and the value is the list of shared edges.

For example:

```text
Face 1 <-> Face 10 via Edge 1 Edge 3
```

means that two different edges connect the same pair of faces.

Preserving the edge IDs is useful because the adjacency relationship retains its topological provenance.

---

# Seam edges

A particularly important B-rep case appears in `cube_hole.step`.

The output contains:

```text
Edge 15: F7 F7
```

This does **not** mean that two different faces are connected.

Instead, the same cylindrical face occurs twice in the edge-to-face relationship.

This is a seam edge.

Therefore, when constructing face adjacency, the implementation removes duplicate face IDs before generating face pairs.

Without this step, the program could incorrectly interpret:

```text
F7 F7
```

as a connection from Face 7 to itself.

---

# Example: cube with a cylindrical hole

For `cube_hole.step` the indexed topology is:

```text
Faces:    7
Edges:    15
Vertices: 10
```

The face adjacency contains relationships such as:

```text
Face 1 <-> Face 2 via Edge 1
Face 1 <-> Face 3 via Edge 4
Face 1 <-> Face 4 via Edge 3
Face 1 <-> Face 5 via Edge 2
```

and:

```text
Face 3 <-> Face 7 via Edge 10
Face 5 <-> Face 7 via Edge 14
```

Here Face 7 is the cylindrical hole surface.

This demonstrates how geometric features can eventually be detected from topology.

For example, a future feature-recognition algorithm could identify:

```text
cylindrical face
    +
two planar/capping relationships
    +
closed topology
        ↓
candidate cylindrical hole
```

That analysis belongs to the later feature-recognition exercise.

---

# Example: PLATE.STEP

For `PLATE.STEP`:

```text
Faces:    10
Edges:    24
Vertices: 16
```

An interesting case is:

```text
Face 1 <-> Face 10 via Edge 1 Edge 3
```

Two faces share two edges.

This demonstrates why face adjacency should not simply be represented as:

```cpp
std::set<std::pair<int,int>>
```

if we want to retain topological provenance.

Instead, the implementation stores:

```text
(Face A, Face B)
        ↓
shared edge IDs
```

allowing:

```text
(F1, F10) → [E1, E3]
```

---

# Topology consistency checks

The program performs basic consistency checks.

It verifies that:

### Face → Edge references are valid

Every edge referenced by a face must exist in `edgeMap`.

### Edge → Vertex references are valid

Every vertex referenced by an edge must exist in `vertexMap`.

### Faces contain edges

A face without any edges is reported as invalid.

### Edges contain vertices

An edge without any vertices is reported as invalid.

### Edges have incident faces

Every edge should have at least one incident face.

We deliberately do **not** require exactly two distinct incident faces.

That would incorrectly reject valid special cases:

```text
Boundary edge
    → one face

Seam edge
    → same face twice

Non-manifold edge
    → more than two incident faces
```

These cases will be studied more carefully in the shell/manifold analysis exercise.

---

# Stable topology IDs

The program also verifies that topology references can be mapped back to the indexed maps.

For example:

```text
Face
 ↓
Edge
 ↓
edgeMap.FindIndex()
 ↓
Edge ID
```

and:

```text
Edge
 ↓
Vertex
 ↓
vertexMap.FindIndex()
 ↓
Vertex ID
```

The final output:

```text
All topology references map to stable IDs.
```

confirms that the internal incidence representation is consistent.

---

# Why this matters for CAD geometry systems

This exercise is an important transition from CAD file parsing to structured geometric reasoning.

The model is no longer treated simply as:

```text
STEP file → collection of shapes
```

Instead, it becomes:

```text
             Face
            /    \
           /      \
        Edge ---- Edge
        /  \       /  \
       /    \     /    \
   Vertex   Vertex   Vertex
```

or, more formally, a graph of topological entities and relationships.

This representation is useful for:

* feature recognition
* geometric reasoning
* CAD similarity
* shape matching
* mating detection
* graph-based machine learning
* CAD foundation-model datasets
* topology-aware reconstruction

The next exercises will add geometric information to this topological structure.

---

# Pipeline

The current portfolio progression is:

```text
STEP
 ↓
STEPControl_Reader
 ↓
TopoDS_Shape
 ↓
Indexed topology
 ↓
Face / Edge / Vertex IDs
 ↓
Incidence relationships
 ↓
Face adjacency
 ↓
B-rep graph
```

The next step is to classify edges using both topology and underlying geometry.

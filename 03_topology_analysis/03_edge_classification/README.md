# Edge Classification

This exercise analyzes the edges of a STEP-based B-rep model and classifies each edge using both its underlying geometric curve and its topological relationships.

The goal is to move from simply traversing B-rep topology to extracting structured geometric information that can later support feature recognition, B-rep graphs, CAD similarity, and machine-learning representations.

---

## Learning Objectives

This exercise demonstrates how to:

* Load a STEP model with OpenCASCADE
* Build stable indexed maps of edges and vertices
* Access the geometric curve underlying a `TopoDS_Edge`
* Classify curves by geometric type
* Compute edge lengths
* Identify the vertices belonging to each edge
* Identify the faces incident to each edge
* Distinguish geometric information from topological information

---

## Concept

An OCCT edge is a **topological entity**.

The geometry associated with that edge is represented by a curve:

```text
TopoDS_Edge
     │
     │ BRep_Tool::Curve()
     ▼
Geom_Curve
     │
     ├── Geom_Line
     ├── Geom_Circle
     ├── Geom_Ellipse
     ├── Geom_Parabola
     ├── Geom_Hyperbola
     ├── Geom_BezierCurve
     └── Geom_BSplineCurve
```

This distinction is important:

* **Topology** describes how entities are connected.
* **Geometry** describes their mathematical shape.

For example, an edge may be topologically connected to two faces while its underlying geometry is a circle.

---

## Edge Information

For each edge, the program extracts:

```text
Edge
 ├── ID
 ├── Curve type
 ├── Length
 ├── Start/end vertices
 └── Incident faces
```

A typical result looks like:

```text
Edge 10
  Curve type: Circle
  Length: ...
  Vertices: V8 V8
  Faces: F3 F7
```

The exact IDs and values depend on the input STEP model.

---

## Curve Classification

The current implementation recognizes common OCCT curve types:

| Curve type | OCCT class               |
| ---------- | ------------------------ |
| Line       | `Geom_Line`              |
| Circle     | `Geom_Circle`            |
| Ellipse    | `Geom_Ellipse`           |
| Parabola   | `Geom_Parabola`          |
| Hyperbola  | `Geom_Hyperbola`         |
| Bezier     | `Geom_BezierCurve`       |
| BSpline    | `Geom_BSplineCurve`      |
| Other      | Other `Geom_Curve` types |

The classification is performed using OCCT's dynamic type system:

```cpp
if (!curve.IsNull())
{
    if (!Handle(Geom_Line)::DownCast(curve).IsNull())
        return "Line";

    if (!Handle(Geom_Circle)::DownCast(curve).IsNull())
        return "Circle";

    // ...
}
```

This allows the program to determine the mathematical representation of the edge.

---

## Edge Length

The underlying curve is accessed with:

```cpp
BRep_Tool::Curve(edge, first, last);
```

However, in OCCT 7.9, `GCPnts_AbscissaPoint::Length()` operates on an `Adaptor3d_Curve`.

Therefore the implementation uses:

```cpp
BRepAdaptor_Curve adaptor(edge);

double length =
    GCPnts_AbscissaPoint::Length(
        adaptor,
        adaptor.FirstParameter(),
        adaptor.LastParameter());
```

This is an important OCCT concept:

```text
TopoDS_Edge
     │
     ▼
BRepAdaptor_Curve
     │
     ▼
GCPnts_AbscissaPoint
     │
     ▼
Curve length
```

The adaptor provides a common interface for numerical operations on different curve representations.

---

## Topological Relationships

The program also extracts the topological relationships associated with each edge.

### Edge → Vertices

Each edge is associated with its endpoint vertices.

For example:

```text
Edge 1
  Vertices: V1 V2
```

The vertex IDs come from an indexed topology map:

```cpp
TopTools_IndexedMapOfShape vertexMap;

TopExp::MapShapes(
    shape,
    TopAbs_VERTEX,
    vertexMap);
```

An edge can then be associated with its vertices using:

```cpp
TopExp_Explorer explorer(
    edge,
    TopAbs_VERTEX);
```

---

### Edge → Faces

An edge can also be associated with the faces that use it.

The relationship is constructed using:

```cpp
TopExp::MapShapesAndAncestors(
    shape,
    TopAbs_EDGE,
    TopAbs_FACE,
    edgeFaceMap);
```

This produces a relationship of the form:

```text
Edge
 ├── Face
 └── Face
```

For a normal shared edge, two distinct faces commonly reference the edge.

However, an edge can also have special topological behavior, such as a **seam edge**, where the same face appears on both sides of the edge.

Therefore, code should not blindly assume that the two incident faces are always distinct.

---

## Geometric vs. Topological Classification

This exercise intentionally separates two different kinds of information.

### Geometric classification

Answers:

> What mathematical curve is this edge?

Examples:

```text
Line
Circle
Ellipse
BSpline
...
```

### Topological classification

Answers:

> How is this edge connected to the rest of the B-rep?

Examples include:

```text
Boundary edge
Shared edge
Seam edge
Degenerate edge
```

These classifications are related, but they are not the same.

For example:

```text
Circle + shared edge
Circle + seam edge
Line + shared edge
Line + boundary edge
```

are all possible combinations.

Keeping geometry and topology separate is important when building higher-level CAD representations.

---

## Indexed Topology

The exercise uses OCCT indexed maps to give topology entities stable IDs during analysis:

```cpp
TopTools_IndexedMapOfShape edgeMap;
TopTools_IndexedMapOfShape vertexMap;
```

with:

```cpp
TopExp::MapShapes(
    shape,
    TopAbs_EDGE,
    edgeMap);

TopExp::MapShapes(
    shape,
    TopAbs_VERTEX,
    vertexMap);
```

This allows relationships to be represented using compact integer IDs:

```text
Edge 3 → V3 V4
Edge 3 → F1 F4
```

These IDs are local to the analyzed shape and are useful for constructing structured representations such as:

```text
Edge → Vertices
Edge → Faces
Face → Edges
```

They also provide a foundation for the B-rep graph developed later in the project.

---

## Example Output

The program produces output similar to:

```text
EDGE CLASSIFICATION
===================

Edges: ...

Edge 1
  Curve type: Line
  Length: ...
  Vertices: V1 V2
  Faces: F1 F2

Edge 2
  Curve type: Circle
  Length: ...
  Vertices: V3 V4
  Faces: F1 F3

...
```

The output is intentionally simple. The important part is the structured information being extracted from the B-rep.

---

## Why This Matters for CAD Geometry

Edge classification is a basic building block for higher-level CAD reasoning.

For example, a system could combine:

```text
Edge geometry
      +
Edge topology
      +
Adjacent face geometry
      +
Edge length
      +
Vertex relationships
```

to identify geometric patterns.

A possible progression is:

```text
Raw STEP
   ↓
B-rep topology
   ↓
Edge classification
   ↓
Face relationships
   ↓
Feature candidates
   ↓
B-rep graph
   ↓
CAD representation
```

This is particularly relevant to CAD geometry pipelines because geometric entities alone are not sufficient. Their relationships and connectivity are equally important.

---

## Current Scope

This exercise currently focuses on:

* Curve type
* Curve length
* Edge vertices
* Incident faces

It does **not** yet attempt full feature recognition.

The following classifications are intentionally left for subsequent exercises:

* Boundary vs. shared edges
* Seam edges
* Degenerate edges
* Convex vs. concave edges
* Tangent edges
* Geometric relationships between adjacent faces

These will build on the topology established here.

---

## OpenCASCADE Concepts Used

Main OCCT classes and APIs:

```text
STEPControl_Reader
TopoDS_Shape
TopoDS_Edge
TopExp_Explorer
TopExp
TopTools_IndexedMapOfShape
TopTools_IndexedDataMapOfShapeListOfShape

BRep_Tool
BRepAdaptor_Curve
GCPnts_AbscissaPoint

Geom_Curve
Geom_Line
Geom_Circle
Geom_Ellipse
Geom_Parabola
Geom_Hyperbola
Geom_BezierCurve
Geom_BSplineCurve
```

---

## Key Takeaway

The important concept in this exercise is the distinction between **topological edges** and their **underlying geometric curves**.

An edge is not simply "a line" or "a circle." It is a topological entity that references geometry and participates in relationships with vertices and faces.

That distinction provides the foundation for the next stages of the project:

```text
Geometry
   +
Topology
   +
Relationships
   ↓
CAD reasoning
```

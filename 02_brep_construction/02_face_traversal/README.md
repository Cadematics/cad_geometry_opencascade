# Exercise 2 — Face Construction and Topology Traversal

This exercise introduces the relationship between the main B-rep topological entities in OpenCASCADE:

```text
Face
 └── Edge
      └── Vertex
```

The goal is to construct a simple planar face and then inspect its topology using `TopExp_Explorer`.

---

## Objective

Build a rectangular planar face and:

1. Construct its boundary from vertices and edges.
2. Create a `TopoDS_Face`.
3. Traverse the edges belonging to the face.
4. Traverse the vertices belonging to the face.
5. Inspect the underlying geometric surface.

This establishes the basic connection between **B-rep topology** and **geometry**.

---

## B-rep Structure

A boundary-representation model describes a solid using topological entities.

For this exercise:

```text
TopoDS_Face
    │
    ├── TopoDS_Edge
    │       ├── TopoDS_Vertex
    │       └── TopoDS_Vertex
    │
    ├── TopoDS_Edge
    │       ├── TopoDS_Vertex
    │       └── TopoDS_Vertex
    │
    └── ...
```

The face represents a bounded region, while its edges define the boundary of that region.

---

## Construction

The rectangular face is constructed using:

```cpp
BRepBuilderAPI_MakePolygon
BRepBuilderAPI_MakeFace
```

The basic workflow is:

```text
Points
  ↓
Closed wire
  ↓
Face
```

For example, a rectangular boundary can be defined by four points:

```text
P4 ───────────── P3
│                │
│                │
│                │
P1 ───────────── P2
```

The wire is closed by connecting the final point back to the first point.

---

## Topology Traversal

OpenCASCADE provides `TopExp_Explorer` for traversing the topology of a shape.

For example:

```cpp
TopExp_Explorer explorer(face, TopAbs_EDGE);
```

iterates over the edges of the face.

Similarly:

```cpp
TopExp_Explorer explorer(face, TopAbs_VERTEX);
```

iterates over its vertices.

The general pattern is:

```text
TopoDS_Shape
      ↓
TopExp_Explorer
      ↓
Topological entity
```

This same approach will later be used to traverse complete solids:

```text
Solid
 ↓
Shell
 ↓
Face
 ↓
Edge
 ↓
Vertex
```

---

## Geometry vs. Topology

This exercise introduces an important CAD concept.

### Topology

Describes **connectivity and relationships**:

```text
Face
Edge
Vertex
```

### Geometry

Describes the mathematical shape:

```text
Plane
Cylinder
Sphere
Line
Circle
Point
```

A face therefore has both:

```text
TopoDS_Face
     │
     └── underlying geometric surface
```

For a rectangular face, the underlying surface is a plane.

It can be accessed with:

```cpp
BRep_Tool::Surface(face)
```

and represented by a `Geom_Surface`.

---

## Key OpenCASCADE Classes

### `BRepBuilderAPI_MakePolygon`

Constructs a wire from a sequence of points.

### `BRepBuilderAPI_MakeFace`

Creates a face from a closed wire.

### `TopoDS_Face`

Represents the topological face.

### `TopExp_Explorer`

Traverses entities contained within a topological shape.

### `BRep_Tool`

Provides access to geometric information associated with B-rep entities.

---

## Build

Using the system OpenCASCADE installation:

```bash
g++ -std=c++17 \
    main.cpp \
    -I/usr/local/include/opencascade \
    -L/usr/local/lib \
    -Wl,-rpath,/usr/local/lib \
    -lTKTopAlgo \
    -lTKBRep \
    -lTKGeomBase \
    -lTKMath \
    -lTKernel \
    -o face_traversal
```

---

## Run

```bash
./face_traversal
```

The program should report that the face was successfully created and then list its boundary edges and vertices.

For a simple rectangular face, the expected topology is:

```text
1 Face
4 Edges
4 unique geometric corner points
```

Note that topology traversal can expose repeated vertex occurrences depending on how the topology is represented. A topological occurrence should not automatically be interpreted as a unique geometric point.

---

## What This Exercise Demonstrates

This exercise establishes the basic B-rep hierarchy used throughout the rest of the project:

```text
Geometry
   ↑
B-rep entities
   ↑
Topology
```

More specifically:

```text
Face
 ↓
Edges
 ↓
Vertices
```

Understanding this relationship is essential before working with larger CAD models, STEP files, B-rep graphs, feature recognition, and geometric reasoning.

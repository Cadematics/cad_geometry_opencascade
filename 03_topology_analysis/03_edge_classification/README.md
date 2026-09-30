# 03 — Edge Classification

This exercise analyzes the edges of a B-rep solid imported from a STEP file.

The goal is to connect **topology** with the **underlying geometric representation** of each edge.

For every edge, the program reports:

* Edge ID
* Underlying curve type
* Curve length
* Start/end vertex IDs
* Incident face IDs

This provides the foundation for more advanced CAD reasoning such as boundary detection, seam detection, convex/concave edge classification, feature recognition, and B-rep graph construction.

---

## 1. Concept

In OpenCASCADE, an edge is a **topological entity** that references an underlying geometric curve.

The relationship is approximately:

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

The important distinction is:

```text
Topology                         Geometry
--------                         --------
Edge                             Curve
Vertex                           Point
Face                             Surface
```

An edge therefore contains topological information such as which faces and vertices it connects, while its underlying curve describes its geometric shape.

For example:

```text
Edge
 ├── Curve: Circle
 ├── Length: 31.4159
 ├── Vertices: V8 V8
 └── Faces: F3 F7
```

The repeated vertex ID is expected for a circular seam edge. A closed geometric curve can have the same topological vertex at both ends.

---

## 2. What This Exercise Demonstrates

The program combines information from several parts of the OpenCASCADE geometry/topology model.

### Curve classification

The underlying `Geom_Curve` is inspected using its dynamic type.

Currently supported classifications are:

```text
Line
Circle
Ellipse
Parabola
Hyperbola
Bezier
BSpline
Other
```

### Curve length

Curve length is computed using:

```cpp
BRepAdaptor_Curve
```

together with:

```cpp
GCPnts_AbscissaPoint::Length()
```

The adaptor is used because the OCCT 7.9 API expects an `Adaptor3d_Curve` for this calculation.

### Topological connectivity

The program also builds indexed maps of:

```text
Face
Edge
Vertex
```

and determines:

```text
Edge → Vertices
Edge → Faces
```

This makes it possible to reason about both the geometry and topology of an edge.

---

## 3. Example

For a box with a cylindrical through-hole, the output contains both linear and circular edges:

```text
Edge 1
  Curve type: Line
  Length: 20
  Vertices: V1 V2
  Faces: F1 F2

...

Edge 10
  Curve type: Circle
  Length: 31.4159
  Vertices: V8 V8
  Faces: F3 F7
```

The circular edge has:

```text
Length ≈ π × diameter
      ≈ π × 10
      ≈ 31.4159
```

The repeated vertex:

```text
V8 V8
```

indicates that the circular edge is closed in the underlying geometry.

Another example is an edge that belongs to only one face:

```text
Faces: F7
```

Such an edge would be a candidate for a boundary edge. This distinction will be used in a later exercise.

---

## 4. Building

This project uses the system OpenCASCADE installation.

Expected environment:

```text
OpenCASCADE 7.9.x
g++ 13+
C++17
```

From this directory:

```bash
g++ -std=c++17 \
    main.cpp \
    -I/usr/local/include/opencascade \
    -L/usr/local/lib \
    -Wl,-rpath,/usr/local/lib \
    -l:libTKDESTEP.so.7.9.3 \
    -l:libTKXSBase.so.7.9.3 \
    -l:libTKTopAlgo.so.7.9.3 \
    -l:libTKBRep.so.7.9.3 \
    -l:libTKGeomBase.so.7.9.3 \
    -l:libTKG3d.so.7.9.3 \
    -l:libTKMath.so.7.9.3 \
    -l:libTKernel.so.7.9.3 \
    -o edge_classification
```

The `rpath` option allows the executable to find the OpenCASCADE libraries in:

```text
/usr/local/lib
```

without requiring `LD_LIBRARY_PATH`.

---

## 5. Running

The program accepts the STEP file path as a command-line argument.

### Generic usage

```bash
./edge_classification path/to/model.step
```

For example:

```bash
./edge_classification ../../models/cube_hole.step
```

or:

```bash
./edge_classification ../../models/PLATE.STEP
```

The program is not tied to either of these models. Any compatible STEP file can be supplied:

```bash
./edge_classification /path/to/your/model.step
```

---

## 6. Output

The output has the following general structure:

```text
EDGE CLASSIFICATION
===================

Edges: N

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

The IDs are generated from the topology maps created for the imported STEP model.

They are useful for relating this exercise to the previous topology-analysis exercises.

---

## 7. Relationship to Previous Exercises

This exercise builds directly on the previous B-rep topology work.

### Previous exercise

`02_incidence_and_adjacency`

Established:

```text
Face → Edges
Edge → Faces
Edge → Vertices
Vertex → Edges
Face ↔ Face
```

### This exercise

Adds geometric information:

```text
Edge
 ├── Curve type
 ├── Length
 ├── Vertices
 └── Faces
```

The resulting representation is therefore:

```text
             B-REP
               │
        ┌──────┴──────┐
        │             │
     Topology      Geometry
        │             │
      Edge        Geom_Curve
        │             │
   ┌────┴────┐   ┌────┴─────┐
   │         │   │          │
Vertices   Faces Line      Circle
                       ...
```

This is an important step toward treating CAD models as structured data rather than simply as meshes or collections of triangles.

---

## 8. Important CAD Concepts

### Topological edge vs geometric curve

A `TopoDS_Edge` is not simply a mathematical curve.

It is a topological entity that references geometric information.

For example, several topological edges can reference geometrically similar curves, while their topological role in the B-rep can be different.

This distinction becomes important when analyzing CAD models.

---

### Closed curves and seam edges

A circular surface is periodic.

Consequently, an edge representing a circular seam may have the same vertex at both ends:

```text
V8 → V8
```

For example:

```text
Edge 10
  Curve type: Circle
  Vertices: V8 V8
```

This should not automatically be interpreted as an invalid zero-length edge.

The geometric curve can still have a finite length:

```text
Length = 2πr
```

---

### Edge incidence

An edge can have different topological relationships with faces.

For example:

```text
Edge → F1 F2
```

means the edge is shared by two faces.

A later exercise will use this information to distinguish:

```text
Boundary edge
Shared edge
Seam edge
Degenerate edge
```

These classifications are more meaningful for CAD reasoning than curve type alone.

---

## 9. Current Scope

The current implementation intentionally focuses on two layers:

### Geometric classification

```text
Line
Circle
Ellipse
Parabola
Hyperbola
Bezier
BSpline
Other
```

### Basic topology

```text
Edge → Vertices
Edge → Faces
```

It does **not** yet attempt to determine:

* boundary vs shared edges
* seam edges
* degenerate edges
* convex vs concave edges
* tangent edges
* feature boundaries
* geometric continuity
* edge curvature
* feature semantics

Those are left for subsequent topology-analysis exercises.

---

## 10. Next Step

The next stage is to classify the **topological role** of each edge.

The planned progression is:

```text
03_edge_classification
        │
        ▼
Curve type + length + connectivity
        │
        ▼
Boundary / Shared / Seam / Degenerate
        │
        ▼
Face relationships
        │
        ▼
Shell and manifold analysis
        │
        ▼
Feature candidates
        │
        ▼
B-rep graph
```

The important transition is from:

> "What geometric curve is this?"

to:

> "What role does this edge play in the B-rep?"

That distinction is fundamental for higher-level CAD reasoning and feature recognition.

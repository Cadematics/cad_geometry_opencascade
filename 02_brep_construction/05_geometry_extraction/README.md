# Exercise 5 — Geometry Extraction

This exercise extracts the **underlying geometric surface** of each B-rep face from a STEP file using OpenCASCADE.

The goal is to move from:

```text
STEP file
   ↓
TopoDS_Shape
   ↓
TopoDS_Face
   ↓
Geom_Surface
   ↓
Surface classification
   ↓
Geometric parameters
```

This is the final exercise in the `02_brep_construction` section and establishes the connection between **B-rep topology** and **underlying CAD geometry**.

---

## Objective

For every face in a STEP model:

1. Load the STEP file.
2. Traverse all B-rep faces.
3. Extract the underlying `Geom_Surface`.
4. Identify its geometric type.
5. Extract useful geometric parameters.

The program recognizes:

* Plane
* Cylinder
* Sphere
* Cone
* Torus
* Other surface types

---

## Key OpenCASCADE Classes

### STEP import

```cpp
STEPControl_Reader
```

Used to read and transfer STEP geometry into an OpenCASCADE B-rep.

### B-rep topology

```cpp
TopoDS_Shape
TopoDS_Face
TopExp_Explorer
```

Used to represent and traverse the model topology.

### Geometry extraction

```cpp
BRep_Tool::Surface()
```

The key operation is:

```cpp
Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
```

A `TopoDS_Face` represents a **topological face**, while `Geom_Surface` represents the underlying mathematical surface.

---

## Surface Classification

The general structure is:

```text
TopoDS_Face
     │
     └── Geom_Surface
           │
           ├── Geom_Plane
           ├── Geom_CylindricalSurface
           ├── Geom_SphericalSurface
           ├── Geom_ConicalSurface
           └── Geom_ToroidalSurface
```

OpenCASCADE's runtime type system is used to determine the concrete surface type:

```cpp
surface->IsKind(STANDARD_TYPE(Geom_Plane))
```

The surface can then be safely converted using `DownCast`.

---

## Extracted Parameters

### Plane

The program extracts:

```text
Origin
Normal
```

A plane can be described by:

$$
\mathbf{n}\cdot(\mathbf{x}-\mathbf{p}_0)=0
$$

where:

* \(\mathbf{p}_0\) is a point on the plane
* \(\mathbf{n}\) is the plane normal

---

### Cylinder

The program extracts:

```text
Radius
Axis origin
Axis direction
```

A cylinder is defined by its radius and axis.

---

### Sphere

The program extracts:

```text
Radius
```

A sphere is defined by:

$$
\|\mathbf{x}-\mathbf{c}\|=R
$$

where \(R\) is the radius.

---

### Cone

The program extracts:

```text
Semi-angle
Reference radius
```

---

### Torus

The program extracts:

```text
Major radius
Minor radius
```

---

## Test Models

Two STEP files are used.

### `cube_hole.step`

A 20 × 20 × 20 mm cube with a cylindrical through-hole.

The B-rep contains:

```text
7 faces
6 planar surfaces
1 cylindrical surface
```

The extracted cylindrical surface has:

```text
Radius: 5
Axis origin: 10, 10, -1
Axis direction: 0, 0, 1
```

This corresponds to the cylindrical through-hole created in the previous exercise.

---

### `PLATE.STEP`

A more complex plate-like STEP model.

The B-rep contains:

```text
10 faces
6 planar surfaces
4 cylindrical surfaces
```

The cylindrical surfaces have radius:

```text
Radius: 5
```

Their axes are oriented along the Y direction.

The extracted geometry demonstrates that multiple topological faces can reference cylindrical surfaces with different spatial locations.

---

## Results

### `cube_hole.step`

```text
Surface classification
----------------------

Faces:                  7
Planar surfaces:        6
Cylindrical surfaces:   1
```

The model correctly identifies the six planar exterior faces and the cylindrical surface representing the through-hole.

---

### `PLATE.STEP`

```text
Surface classification
----------------------

Faces:                  10
Planar surfaces:        6
Cylindrical surfaces:   4
```

The program correctly identifies both planar and cylindrical regions of the model.

---

## Example Output

For `cube_hole.step`:

```text
Face 7
------
Surface type: Cylinder
Radius: 5
Axis origin: 10, 10, -1
Axis direction: 0, 0, 1
```

For `PLATE.STEP`:

```text
Face 1
------
Surface type: Cylinder
Radius: 5
Axis origin: 40, -7.5, 0
Axis direction: -0, 1, -0
```

The `-0` values are numerical representations of zero and do not indicate a different geometric direction.

---

## Build

Using the system OpenCASCADE 7.9.3 installation:

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
    -o geometry_extraction
```

---

## Run

```bash
./geometry_extraction cube_hole.step
```

and:

```bash
./geometry_extraction PLATE.STEP
```

---

## Topology vs. Geometry

This exercise reinforces one of the most important concepts in B-rep modeling.

### Topology

Describes connectivity:

```text
Solid
 └── Shell
      └── Face
           └── Edge
                └── Vertex
```

### Geometry

Describes mathematical entities:

```text
Face  → Plane / Cylinder / Sphere / ...
Edge  → Line / Circle / ...
Vertex → Point
```

Therefore a CAD model can be viewed as:

```text
B-rep
 ├── Topology
 │    ├── Solids
 │    ├── Shells
 │    ├── Faces
 │    ├── Edges
 │    └── Vertices
 │
 └── Geometry
      ├── Surfaces
      ├── Curves
      └── Points
```

This distinction is fundamental to geometric reasoning.

---

## Why This Matters for CAD Geometry

Surface classification provides information that cannot be obtained from topology alone.

For example:

```text
Face A → Plane
Face B → Plane
Face C → Cylinder
```

allows a geometry-processing system to reason about relationships such as:

```text
planar / cylindrical
parallel / perpendicular
same radius
same axis
coaxial
coplanar
```

These relationships become useful for:

* feature recognition
* machining feature extraction
* mating detection
* geometric constraints
* CAD similarity
* B-rep graph construction
* parametric CAD reconstruction
* ML-ready CAD representations

---

## Position in the Portfolio Pipeline

This exercise completes the initial B-rep construction and inspection pipeline:

```text
STEP
 ↓
B-rep
 ↓
Topology
 ↓
Faces / Edges / Vertices
 ↓
Underlying Geometry
 ↓
Surface Classification
 ↓
Geometric Parameters
```

The next section will build on this information by analyzing **relationships between topological entities**, including face adjacency, shared edges, and geometric relationships.

Longer term, the portfolio will progress toward:

```text
CAD kernel
    ↓
B-rep
    ↓
Topology
    ↓
Geometry
    ↓
Verification
    ↓
B-rep graph
    ↓
Feature recognition
    ↓
Reconstruction
    ↓
CAD similarity
    ↓
ML-ready representation
```

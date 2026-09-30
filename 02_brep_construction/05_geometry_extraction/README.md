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

This is an important step toward building CAD geometry pipelines that operate on both **topology** and **geometry**.

---

## Objective

For every face in a STEP model:

1. Load the STEP file.
2. Traverse all B-rep faces.
3. Extract the underlying `Geom_Surface`.
4. Identify its geometric type.
5. Extract useful parameters from the surface.

The exercise currently recognizes:

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

This is the key operation in this exercise:

```cpp
Handle(Geom_Surface) surface = BRep_Tool::Surface(face);
```

A `TopoDS_Face` represents a **topological face**, while the `Geom_Surface` represents the underlying mathematical surface on which that face lies.

For example:

```text
TopoDS_Face
    │
    └── Geom_Surface
          ├── Geom_Plane
          ├── Geom_CylindricalSurface
          ├── Geom_SphericalSurface
          ├── Geom_ConicalSurface
          └── Geom_ToroidalSurface
```

---

## Why This Matters

A B-rep is not only a collection of faces and edges.

Each topological entity is associated with underlying geometric entities.

For example, a cylindrical hole may appear in the topology as:

```text
Solid
 └── Shell
      └── Face
           └── Cylindrical Surface
                ├── radius
                └── axis
```

Being able to recover these geometric properties is fundamental for:

* CAD feature recognition
* geometric reasoning
* face classification
* machining feature extraction
* mating detection
* CAD similarity
* B-rep graphs
* parametric CAD reconstruction
* ML-ready CAD representations

---

## Surface Parameters

### Plane

For a planar face we extract:

```text
Origin
Normal
```

Represented by:

```cpp
Geom_Plane
gp_Pln
```

Conceptually:

$$
\mathbf{n}\cdot(\mathbf{x}-\mathbf{p}_0)=0
$$

where:

* \(\mathbf{p}_0\) is a point on the plane
* \(\mathbf{n}\) is the plane normal

---

### Cylinder

For a cylindrical face we extract:

```text
Radius
Axis origin
Axis direction
```

Represented by:

```cpp
Geom_CylindricalSurface
```

A cylinder can be described by:

$$
\|\mathbf{x}-\mathbf{c}(t)\|=R
$$

where \(R\) is the cylinder radius and the centerline is defined by the cylinder axis.

---

### Sphere

For a spherical face we extract:

```text
Radius
```

Represented by:

```cpp
Geom_SphericalSurface
```

A sphere is defined by:

$$
\|\mathbf{x}-\mathbf{c}\|=R
$$

where:

* \(\mathbf{c}\) is the center
* \(R\) is the radius

---

### Cone

For a conical face we extract:

```text
Semi-angle
Reference radius
```

Represented by:

```cpp
Geom_ConicalSurface
```

The cone is defined by an axis, a reference point/radius, and a semi-angle.

---

### Torus

For a toroidal face we extract:

```text
Major radius
Minor radius
```

Represented by:

```cpp
Geom_ToroidalSurface
```

A torus is characterized by:

* major radius \(R\)
* minor radius \(r\)

---

## Surface Classification

The program uses OpenCASCADE's runtime type system:

```cpp
surface->IsKind(STANDARD_TYPE(Geom_Plane))
```

and then safely converts the surface:

```cpp
Handle(Geom_Plane) plane =
    Handle(Geom_Plane)::DownCast(surface);
```

This allows the same `Geom_Surface` interface to be classified into its concrete geometric type.

Conceptually:

```text
Geom_Surface
     │
     ├── Plane
     ├── Cylinder
     ├── Sphere
     ├── Cone
     ├── Torus
     └── Other
```

---

## Example Models

This exercise uses two STEP models:

### `cube_hole.step`

A 20 × 20 × 20 mm cube with a cylindrical through-hole.

Its geometry should contain:

```text
Planar faces
+
Cylindrical face
```

This provides a simple example where topology alone is not sufficient to understand the shape.

### `PLATE.STEP`

A more complex plate-like model containing multiple planar and non-planar geometric regions.

The model is useful for testing the surface-classification code against a less trivial B-rep.

---

## Build

The project uses the system OpenCASCADE installation:

```text
/usr/local/include/opencascade
/usr/local/lib
```

Build with:

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

For the cube with a cylindrical hole:

```bash
./geometry_extraction cube_hole.step
```

For the plate:

```bash
./geometry_extraction PLATE.STEP
```

---

## Expected Processing Pipeline

```text
                 STEP
                  │
                  ▼
        STEPControl_Reader
                  │
                  ▼
           TopoDS_Shape
                  │
                  ▼
          TopExp_Explorer
                  │
                  ▼
            TopoDS_Face
                  │
                  ▼
       BRep_Tool::Surface()
                  │
                  ▼
           Geom_Surface
                  │
          ┌───────┼────────┐
          ▼       ▼        ▼
        Plane  Cylinder   Sphere
          │       │        │
          ▼       ▼        ▼
       normal   radius   radius
       origin    axis
```

---

## Connection to CAD Geometry Pipelines

This exercise establishes an important distinction:

```text
Topology                         Geometry
--------                         --------
Solid                            Surface
Shell                            Plane
Face                             Cylinder
Edge                             Sphere
Vertex                           Cone
                                 Torus
```

The topology describes **how entities are connected**.

The geometry describes **where those entities exist in space and what mathematical shapes they represent**.

A useful CAD representation therefore needs both:

```text
B-rep
 ├── topology
 │    ├── faces
 │    ├── edges
 │    └── vertices
 │
 └── geometry
      ├── surfaces
      ├── curves
      └── points
```

This separation is central to later exercises involving B-rep graphs, feature recognition, geometric reasoning, and CAD reconstruction.

---

## Next Step

The next stage will build on this geometry information by adding **geometric and topological verification**.

The longer-term pipeline is:

```text
STEP
 ↓
B-rep
 ↓
Topology
 ↓
Geometry extraction
 ↓
Verification
 ↓
Feature recognition
 ↓
B-rep graph
 ↓
Reconstruction
 ↓
CAD similarity / ML representation
```

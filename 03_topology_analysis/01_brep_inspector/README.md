# Project 1 — B-rep Inspector

This project builds a small B-rep inspection tool using OpenCASCADE.

The goal is to move beyond simply counting topological entities and create a structured description of a CAD model containing both **topological and geometric information**.

---

## Objective

Given a STEP file, the inspector extracts:

### Model-level topology

* Solids
* Shells
* Faces
* Edges
* Vertices

### Face-level information

* Face ID
* Surface type
* Surface area
* Centroid
* Surface normal for planar faces
* Number of boundary edges

The processing pipeline is:

```text
STEP
 ↓
STEPControl_Reader
 ↓
TopoDS_Shape
 ↓
Topology traversal
 ↓
Face inspection
 ↓
Geometric properties
 ↓
FaceInfo
```

---

## Architecture

Rather than printing information directly while traversing the OCCT model, the program introduces its own data structure:

```cpp
struct FaceInfo
{
    int id;
    std::string surface_type;
    double area;
    gp_Pnt centroid;
    gp_Dir normal;
    int edge_count;
};
```

This creates a separation between:

```text
OpenCASCADE model
       ↓
Data extraction
       ↓
Our representation
       ↓
Analysis / output
```

This architecture will be extended in the following projects.

---

## Surface Classification

Each face is classified using the underlying `Geom_Surface`.

The current implementation recognizes:

```text
Plane
Cylinder
Sphere
Cone
Torus
Other
```

For example:

```text
TopoDS_Face
     ↓
BRep_Tool::Surface()
     ↓
Geom_Surface
     ↓
Geom_Plane
```

or:

```text
TopoDS_Face
     ↓
BRep_Tool::Surface()
     ↓
Geom_CylindricalSurface
```

---

## Geometric Properties

The surface area and centroid are calculated using OpenCASCADE's geometric property tools:

```cpp
GProp_GProps props;

BRepGProp::SurfaceProperties(face, props);

double area = props.Mass();
gp_Pnt centroid = props.CentreOfMass();
```

For planar faces, the underlying plane also provides its normal.

This gives us a combination of:

```text
Topology
+
Geometry
+
Physical/geometric properties
```

rather than topology alone.

---

# Test 1 — `cube_hole.step`

The model is the 20 × 20 × 20 mm cube with a cylindrical through-hole constructed in the previous section.

### Model topology

```text
Solids:   1
Shells:   1
Faces:    7
Edges:    30
Vertices: 60
```

### Face classification

```text
Faces 1–6 → Plane
Face 7    → Cylinder
```

The six planar faces correspond to the exterior surfaces of the cube.

The cylindrical face corresponds to the through-hole.

### Example

The cylindrical face was extracted as:

```text
Surface: Cylinder
Area: 628.319
Centroid: 10, 10, 10
Edges: 4
```

The cylinder has radius 5 mm, consistent with the geometry constructed in the previous exercise.

---

# Test 2 — `PLATE.STEP`

The second test uses a more complex plate-like STEP model.

### Model topology

```text
Solids:   1
Shells:   1
Faces:    10
Edges:    48
Vertices: 96
```

### Face classification

```text
Faces 1–2  → Cylinder
Faces 3–8  → Plane
Faces 9–10 → Cylinder
```

Therefore:

```text
Planar faces:      6
Cylindrical faces: 4
```

The cylindrical surfaces have radius 5 mm.

The planar faces include large top/bottom surfaces as well as the smaller end surfaces.

---

## Example Face Data

One of the planar faces:

```text
Face 7
------
Surface: Plane
Area: 4642.92
Centroid: approximately (0, 7.5, 0)
Normal: (0, 1, 0)
Edges: 8
```

This demonstrates why face-level analysis is more infor

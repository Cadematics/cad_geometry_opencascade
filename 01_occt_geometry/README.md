# 01 — Points, Vectors, and Directions

This exercise introduces the fundamental 3D geometric primitives used by OpenCASCADE (OCCT).

The goal is to understand the difference between:

* Points
* Vectors
* Directions

and how these primitives are used for basic geometric calculations.

---

## Learning Objectives

By the end of this exercise, we should understand how to:

* Represent a point in 3D space
* Represent a vector in 3D space
* Represent a normalized direction
* Calculate vector magnitude
* Calculate the displacement between two points
* Calculate the distance between two points
* Calculate dot products
* Calculate cross products
* Translate a point using a vector

These operations form the mathematical foundation for later work with:

* Coordinate systems
* Curves
* Surfaces
* Surface normals
* Edges
* Faces
* B-rep topology
* Transformations

---

# 1. `gp_Pnt` — Point

`gp_Pnt` represents a point in three-dimensional Cartesian space.

A point is defined by:

$$
P=(x,y,z)
$$

Example:

```cpp
gp_Pnt p(10.0, 20.0, 30.0);
```

The coordinates can be accessed with:

```cpp
p.X();
p.Y();
p.Z();
```

A point represents a **location**.

It does not represent a direction or a displacement.

---

# 2. `gp_Vec` — Vector

`gp_Vec` represents a three-dimensional vector.

A vector is:

$$
\vec{v}=(v_x,v_y,v_z)
$$

Example:

```cpp
gp_Vec v(3.0, 4.0, 0.0);
```

A vector has:

* Direction
* Magnitude

Its magnitude is:

$$
|\vec{v}| =
\sqrt{v_x^2+v_y^2+v_z^2}
$$

For:

$$
\vec{v}=(3,4,0)
$$

we get:

$$
|\vec{v}|=\sqrt{3^2+4^2}=5
$$

In OCCT:

```cpp
v.Magnitude();
```

---

# 3. `gp_Dir` — Direction

`gp_Dir` represents a normalized direction.

A direction has unit magnitude:

$$
|\vec{d}|=1
$$

For example, the vector:

$$
\vec{v}=(3,4,0)
$$

has magnitude:

$$
|\vec{v}|=5
$$

Its normalized direction is:

$$
\vec{d}
= \left(
\frac{3}{5},
\frac{4}{5},
0
\right)
$$

or:

$$
\vec{d}=(0.6,0.8,0)
$$

In OCCT:

```cpp
gp_Dir d(v);
```

OCCT normalizes the vector when constructing the direction.

---

# 4. Point-to-Point Displacement

Given two points:

$$
P_1=(1,2,3)
$$

and:

$$
P_2=(4,6,3)
$$

the displacement from `P1` to `P2` is:

$$
\vec{P_1P_2}=P_2-P_1
$$

Therefore:

$$
\vec{P_1P_2}=(3,4,0)
$$

In OCCT:

```cpp
gp_Pnt p1(1.0, 2.0, 3.0);
gp_Pnt p2(4.0, 6.0, 3.0);

gp_Vec displacement(p1, p2);
```

The components can then be accessed with:

```cpp
displacement.X();
displacement.Y();
displacement.Z();
```

---

# 5. Distance Between Points

The distance between two points is the magnitude of their displacement vector:

$$
d(P_1,P_2)=|P_2-P_1|
$$

For the example above:

$$
d=\sqrt{3^2+4^2+0^2}=5
$$

OCCT provides this directly:

```cpp
double distance = p1.Distance(p2);
```

---

# 6. Dot Product

The dot product between two vectors is:

$$
\vec{a}\cdot\vec{b}
=
a_xb_x+a_yb_y+a_zb_z
$$

For example:

```cpp
gp_Vec a(1.0, 0.0, 0.0);
gp_Vec b(0.0, 1.0, 0.0);

double dot = a.Dot(b);
```

The result is:

```text
0
```

because the vectors are perpendicular.

The dot product is useful for determining relationships between directions.

For example:

$$
\vec{a}\cdot\vec{b}=0
$$

indicates perpendicular vectors.

Later this becomes important when working with:

* Face normals
* Surface orientation
* Edge directions
* Coordinate systems
* Geometric relationships

---

# 7. Cross Product

The cross product produces a vector perpendicular to two input vectors.

Given:

$$
\vec{a}=(1,0,0)
$$

and:

$$
\vec{b}=(0,1,0)
$$

we get:

$$
\vec{a}\times\vec{b}=(0,0,1)
$$

In OCCT:

```cpp
gp_Vec cross = a.Crossed(b);
```

The resulting vector is:

```text
0, 0, 1
```

Cross products are particularly important for computing geometric orientation and normals.

For example, given two directions along the edges of a planar face:

$$
\vec{e_1}
$$

and:

$$
\vec{e_2}
$$

we can calculate a normal:

$$
\vec{n}=\vec{e_1}\times\vec{e_2}
$$

This concept will become important when we start working with faces and B-rep geometry.

---

# 8. Translating a Point

A vector can be used to translate a point.

Given:

$$
P=(1,2,3)
$$

and:

$$
\vec{v}=(10,20,30)
$$

the translated point is:

$$
P' = P+\vec{v}
$$

Therefore:

$$
P'=(11,22,33)
$$

In OCCT:

```cpp
gp_Pnt p3(1.0, 2.0, 3.0);
gp_Vec translation(10.0, 20.0, 30.0);

gp_Pnt p4 = p3.Translated(translation);
```

---

# 9. Important Conceptual Distinction

One of the most important things to understand is that a **point is not a vector**.

Consider:

```text
Point:
P = (10, 20, 30)

Vector:
V = (3, 4, 0)
```

The point describes a **location**.

The vector describes a **displacement**.

We can perform:

```text
Point + Vector → Point
```

and:

```text
Point - Point → Vector
```

Conceptually:

$$
P+\vec{v}=P'
$$

and:

$$
P_2-P_1=\vec{v}
$$

This distinction becomes fundamental when working with CAD geometry.

---

# OCCT Classes Used

| OCCT Class | Purpose                 |
| ---------- | ----------------------- |
| `gp_Pnt`   | 3D point                |
| `gp_Vec`   | 3D vector               |
| `gp_Dir`   | Normalized 3D direction |

Headers:

```cpp
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
```

---

# Build

Currently this exercise can be compiled directly with:

```bash
g++ -std=c++17 \
    main.cpp \
    -I/usr/local/include/opencascade \
    -L/usr/local/lib \
    -lTKMath \
    -lTKernel \
    -o geometry01
```

Run:

```bash
./geometry01
```

Expected output:

```text
Point: 10, 20, 30
Vector: 3, 4, 0
Vector magnitude: 5
Direction: 0.6, 0.8, 0

Displacement:
dx = 3
dy = 4
dz = 0
Magnitude = 5
Distance = 5

Dot product:
a · b = 0

Cross product:
a x b = 0, 0, 1

Point + vector:
Original point: 1, 2, 3
Translated point: 11, 22, 33
```

---

# OCCT Concepts Learned

```text
gp_Pnt
   │
   ├── X / Y / Z
   ├── Distance
   └── Translation
        │
        ▼
gp_Vec
   │
   ├── X / Y / Z
   ├── Magnitude
   ├── Dot product
   ├── Cross product
   └── Displacement
        │
        ▼
gp_Dir
   │
   └── Normalized direction
```

---

# Why This Matters for CAD

These primitives are the foundation of much more complex CAD operations.

Eventually we will use them to reason about:

```text
Points
   ↓
Curves
   ↓
Edges
   ↓
Wires
   ↓
Faces
   ↓
Shells
   ↓
Solids
   ↓
B-rep
```

For example:

* A vertex contains a point.
* An edge is built from a curve.
* A face is supported by a surface.
* Surface orientation involves directions and normals.
* Coordinate systems define how geometry is positioned.
* Transformations move and rotate geometry.
* B-rep topology connects these geometric entities.

Therefore, understanding `gp_Pnt`, `gp_Vec`, and `gp_Dir` is the first step toward understanding the OCCT geometric model.

---

# Next Exercise

**02 — Coordinate Systems**

We will introduce:

```text
gp_Ax1
gp_Ax2
gp_Ax3
```

and learn how OCCT represents:

* Origins
* Axes
* Directions
* Local coordinate systems
* Relationships between coordinate systems

This will prepare us for **transformations** and eventually for positioning actual CAD geometry.

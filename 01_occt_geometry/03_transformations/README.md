# 3D Transformations — OCCT Geometry Fundamentals

This exercise introduces 3D transformations using Open CASCADE Technology (OCCT).

The previous exercises established the basic geometric building blocks:

```text
gp_Pnt
gp_Vec
gp_Dir
gp_Ax1
gp_Ax2
gp_Ax3
```

This project uses those concepts to transform points and directions in 3D space.

The primary OCCT class introduced here is:

```cpp
gp_Trsf
```

`gp_Trsf` represents a geometric transformation such as translation or rotation.

---

## Learning Objectives

This exercise demonstrates how to:

* Create translation transformations
* Create rotations around arbitrary axes
* Transform points
* Transform directions
* Combine rotation and translation
* Transform geometry between coordinate systems
* Verify geometric properties after transformation
* Understand numerical round-off in geometric computations

These operations are fundamental to CAD modeling, assembly positioning, feature placement, and B-rep processing.

---

# 1. What Is a Geometric Transformation?

A transformation maps a geometric object from one position/orientation to another.

Conceptually:

```text
Original geometry
       │
       │ transformation
       ↓
Transformed geometry
```

For a point:

$$
P' = T(P)
$$

where:

* \(P\) is the original point
* \(T\) is the transformation
* \(P'\) is the transformed point

A rigid 3D transformation can generally be written as:

$$
P' = RP + t
$$

where:

* \(R\) is a rotation
* \(t\) is a translation

This equation is fundamental to CAD geometry processing.

---

# 2. Translation

A translation moves an object without changing its orientation.

For example:

```text
P = (1, 2, 3)

v = (10, 20, 30)

P' = P + v

P' = (11, 22, 33)
```

In OCCT:

```cpp
gp_Vec translation(10.0, 20.0, 30.0);

gp_Trsf translationTrsf;
translationTrsf.SetTranslation(translation);
```

The transformation can then be applied to a point:

```cpp
gp_Pnt translatedPoint =
    point.Transformed(translationTrsf);
```

The output is:

```text
Original point: 1, 2, 3

Translat
```

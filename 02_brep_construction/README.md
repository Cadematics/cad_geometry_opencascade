# Coordinate Systems — OCCT Geometry Fundamentals

This exercise introduces coordinate systems and reference frames in Open CASCADE Technology (OCCT).

The goal is to move from individual geometric primitives such as points and directions to the coordinate systems used to describe CAD geometry, transformations, and local reference frames.

## Learning Objectives

This exercise demonstrates how to:

* Create geometric axes with `gp_Ax1`
* Create 3D coordinate systems with `gp_Ax2`
* Create reference frames with `gp_Ax3`
* Define coordinate-system origins and directions
* Extract X, Y, and Z directions from a coordinate system
* Construct custom axes and local coordinate systems
* Check perpendicularity between directions
* Compare coordinate-system locations and orientations

These concepts form the foundation for CAD transformations, feature placement, part orientation, and B-rep geometry processing.

---

# 1. Coordinate Systems in CAD

A CAD model does not exist only as a collection of points.

Geometry is often described relative to a **coordinate system** or **reference frame**.

A coordinate system consists of:

* An origin
* An X direction
* A Y direction
* A Z direction

For example:

```text
             Z
             ↑
             |
             |
             O────────→ X
            /
           /
          Y
```

The origin defines **where** the coordinate system is located.

The directions define **how it is oriented**.

This becomes particularly important when geometry must be transformed from one coordinate system to another.

---

# 2. `gp_Ax1` — Geometric Axis

`gp_Ax1` represents an axis.

Conceptually:

```text
Axis = Point + Direction
```

Example:

```cpp
gp_Pnt origin(0.0, 0.0, 0.0);
gp_Dir direction(0.0, 0.0, 1.0);

gp_Ax1 zAxis(origin, direction);
```

This represents an axis passing through the origin in the positive Z direction.

A `gp_Ax1` is useful for operations such as:

* Rotations
* Cylindrical geometry
* Symmetry
* Revolutions
* Feature orientation

---

# 3. `gp_Ax2` — Coordinate System

`gp_Ax2` defines a 3D coordinate system.

It contains:

* A location
* A main direction
* An X direction
* A Y direction

For example:

```cpp
gp_Ax2 coordinateSystem(
    origin,
    zDir,
    xDir
);
```

Here:

```text
Z direction = main direction
X direction = X axis
Y direction = derived from X and Z
```

The three directions form an orthogonal coordinate system.

The relationship can be visualized as:

```text
             Z
             ↑
             |
             |
             O────────→ X
            /
           /
          Y
```

OCCT uses the main direction as the coordinate system's Z direction.

---

# 4. `gp_Ax3` — Reference Frame

`gp_Ax3` represents a 3D coordinate system/reference frame.

Example:

```cpp
gp_Ax3 referenceFrame(
    origin,
    zDir,
    xDir
);
```

It provides access to:

```cpp
referenceFrame.Location()
referenceFrame.XDirection()
referenceFrame.YDirection()
referenceFrame.Direction()
```

The last method returns the main direction, which corresponds to the Z direction of the frame.

Reference frames are particularly important when working with:

* Local geometry
* Part coordinate systems
* Transformation matrices
* Feature placement
* Assembly components
* CAD reconstruction

---

# 5. Global vs Local Coordinate Systems

A CAD model can contain multiple coordinate systems.

For example, the global system might be:

```text
Origin = (0, 0, 0)

X = (1, 0, 0)
Y = (0, 1, 0)
Z = (0, 0, 1)
```

A local coordinate system could instead be:

```text
Origin = (100, 50, 25)

X = (1, 0, 0)
Y = (0, 1, 0)
Z = (0, 0, 1)
```

The orientation is the same, but the origin is different.

More generally, a local coordinate system can also be rotated:

```text
Global frame

        Z
        ↑
        |
        O──────→ X


Local frame

             Z'
            ↗
           /
          O'──────→ X'
```

This distinction becomes fundamental when transforming geometry between coordinate systems.

---

# 6. Direction Relationships

The coordinate-system axes must be orthogonal.

For example:

```cpp
xDir.IsNormal(yDir, tolerance)
```

checks whether two directions are perpendicular.

The exercise checks:

```text
X ⟂ Y
X ⟂ Z
Y ⟂ Z
```

with an angular tolerance:

```cpp
const double tolerance = 1e-6;
```

This is important because CAD geometry is numerical. Exact mathematical equality is often inappropriate when comparing geometric entities.

---

# 7. Coordinate Systems and Transformations

Coordinate systems become especially important when applying transformations.

A transformation can involve:

* Translation
* Rotation
* Scaling
* Coordinate-system changes

For example, translating a point:

```text
P = (1, 2, 3)

translation = (10, 20, 30)

P' = (11, 22, 33)
```

This was introduced in Project 1.

The next step is to combine translation and rotation using OCCT's transformation classes.

That will be the focus of the next exercise.

---

# OCCT Classes Used

| Class    | Purpose              |
| -------- | -------------------- |
| `gp_Pnt` | 3D point             |
| `gp_Dir` | Normalized direction |
| `gp_Ax1` | Geometric axis       |
| `gp_Ax2` | 3D coordinate system |
| `gp_Ax3` | 3D reference frame   |

Headers used:

```cpp
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
```

---

# Building the Example

From the project directory:

```bash
g++ -std=c++17 \
    main.cpp \
    -I/usr/local/include/opencascade \
    -L/usr/local/lib \
    -lTKMath \
    -lTKernel \
    -o coordinate_systems
```

Run:

```bash
./coordinate_systems
```

---

# What This Exercise Demonstrates

The exercise builds the following progression:

```text
gp_Pnt
  │
  │ location
  ↓
gp_Dir
  │
  │ orientation
  ↓
gp_Ax1
  │
  │ point + direction
  ↓
gp_Ax2
  │
  │ coordinate system
  ↓
gp_Ax3
  │
  │ reference frame
  ↓
Transformations
```

This progression is important for CAD geometry processing because geometry must be interpreted not only by its shape, but also by its **position and orientation in space**.

---

# Relevance to CAD / Geometry Engineering

Coordinate systems are fundamental to many CAD operations.

They are used when:

* Positioning features
* Orienting parts
* Creating sketches
* Defining machining operations
* Placing assembly components
* Transforming B-rep geometry
* Comparing CAD models
* Reconstructing geometry
* Converting between local and global representations

For a geometry-processing pipeline, a useful mental model is:

```text
Geometry
   +
Topology
   +
Coordinate System
   +
Transformation
   ↓
Geometric representation
```

Understanding reference frames is therefore a prerequisite for more advanced B-rep and CAD processing.

---

# Next Exercise

The next exercise will introduce **transformations** using OCCT's `gp_Trsf`.

Topics will include:

* Translation
* Rotation
* Applying transformations to points
* Applying transformations to directions
* Combining transformations
* Moving geometry between coordinate systems

This will connect the coordinate-system concepts from this exercise to actual CAD transformations.

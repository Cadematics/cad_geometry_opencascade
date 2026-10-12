# OCCT Geometry Fundamentals

## Overview

This module implements foundational geometric operations using Open CASCADE Technology (OCCT) and its geometric primitives. The examples focus on points, vectors, coordinate systems, geometric transformations, and analytic geometry.

The implementations establish a reusable foundation for geometric computation and CAD model processing in C++.

## Examples

### 1. Points and Vectors — `01_points_vectors`

Implements basic 3D geometric calculations using OCCT's `gp_Pnt`, `gp_Vec`, and `gp_Dir` classes.

The example demonstrates:

- Creation and manipulation of 3D points, vectors, and directions.
- Point-to-point displacement and distance calculations.
- Vector magnitude, dot product, and cross product.
- Translation of points using displacement vectors.
- Geometric relationships derived from vector operations.

**Purpose:** Provide basic geometric operations used in CAD algorithms, spatial reasoning, and model analysis.

### 2. Coordinate Systems — `02_coordinate_systems`

Implements 3D coordinate frames using OCCT's `gp_Ax1`, `gp_Ax2`, and `gp_Ax3` classes.

The example demonstrates:

- Definition of axes and local coordinate systems.
- Specification of origins and directional vectors.
- Construction of coordinate frames from geometric directions.
- Inspection of frame orientation and orthogonality.

**Purpose:** Establish coordinate-frame operations for expressing geometry in local reference systems and relating geometric entities to different orientations.

### 3. Geometric Transformations — `03_transformations`

Implements geometric transformations using OCCT's `gp_Trsf` class.

The example demonstrates:

- Translation of geometric entities.
- Rotation around a defined axis.
- Transformation of points and directions.
- Composition of transformations.
- Inspection of transformed coordinates and geometric distances.

**Purpose:** Provide the transformation operations needed to position components, manipulate geometry, and work with local and global coordinate frames.

### 4. Analytic Geometry — `04_analytic_geometry`

Implements calculations involving analytic geometric entities, including lines, planes, and circles.

The example uses OCCT geometry classes such as `gp_Lin`, `gp_Pln`, and `gp_Circ` to investigate geometric properties and relationships.

The operations include:

- Line and plane construction.
- Point-on-line and point-to-line distance calculations.
- Point-to-plane distance and projection.
- Circle evaluation.
- Basic line–plane geometric relationship checks.

**Purpose:** Provide analytic geometry operations that can support geometric constraints, spatial queries, and higher-level CAD analysis.

## Implementation

- **Language:** C++
- **Geometry kernel:** Open CASCADE Technology (OCCT)
- **Primary API:** `gp_Pnt`, `gp_Vec`, `gp_Dir`, `gp_Ax1`, `gp_Ax2`, `gp_Ax3`, `gp_Trsf`, `gp_Lin`, `gp_Pln`, `gp_Circ`

Each example is implemented as a standalone C++ program, with its source in `main.cpp` and its compiled executable in the corresponding directory.

## Role in the CAD Geometry Pipeline

This module provides the geometric foundation for subsequent modules involving B-rep construction, topology analysis, geometric feature recognition, and CAD model processing.

It focuses on geometric primitives and operations rather than boundary representation topology or complete solid models.

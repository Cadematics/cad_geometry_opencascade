# CAD Geometry OpenCASCADE

Progressive exploration of computational geometry, B-rep modeling,
CAD data extraction, topology, reconstruction, and ML-oriented
CAD representations using OpenCASCADE.

## Goal

Build a practical understanding of the CAD kernel and develop a
complete CAD geometry pipeline:

CAD Kernel
    ↓
B-rep
    ↓
Topology
    ↓
Geometry
    ↓
STEP
    ↓
Feature Extraction
    ↓
CAD Representation
    ↓
Reconstruction
    ↓
Verification
    ↓
ML-ready Dataset

## Project Structure

### 01 — OCCT Geometry Fundamentals

Fundamental OpenCASCADE geometry classes:

- gp_Pnt
- gp_Vec
- gp_Dir
- gp_Ax1
- gp_Ax2
- gp_Trsf
- gp_Lin
- gp_Pln
- gp_Circ
- gp_Elips

### 02 — B-rep Construction

Construct CAD models programmatically using OpenCASCADE
topological entities.

### 03 — B-rep Topology Analysis

Inspect solids, shells, faces, edges, and vertices and build
topological relationships.

### 04 — Geometry Extraction

Extract analytical and parametric geometry from B-rep entities.

### 05 — Verification

Calculate and verify geometric and physical properties.

### 06 — STEP Pipeline

Build a CAD ingestion and extraction pipeline around STEP files.

### 07 — B-rep Graph

Represent CAD models as attributed graphs suitable for downstream
analysis and machine learning.

### 08 — Feature Recognition

Recognize CAD features such as holes, pockets, fillets, and chamfers.

### 09 — Reconstruction

Reconstruct CAD geometry from extracted representations.

### 10 — CAD Similarity

Develop numerical CAD descriptors and shape similarity methods.

### 11 — ML-oriented CAD

Convert CAD representations into datasets suitable for machine
learning experiments.

### Final Project

Integrate the complete CAD → B-rep → feature graph →
reconstruction → verification pipeline.

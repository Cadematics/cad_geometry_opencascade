```cpp
#include "face_relationships.h"

#include <algorithm>
#include <cmath>

#include <BRep_Tool.hxx>

#include <Geom_Surface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <gp_Vec.hxx>


// ============================================================
// Relationship → string
// ============================================================

std::string relationshipToString(
    FaceRelationship relationship)
{
    switch (relationship)
    {
        case FaceRelationship::Coplanar:
            return "Coplanar";

        case FaceRelationship::Parallel:
            return "Parallel";

        case FaceRelationship::Perpendicular:
            return "Perpendicular";

        case FaceRelationship::Oblique:
            return "Oblique";

        case FaceRelationship::CylinderAxisPerpendicular:
            return "Cylinder axis perpendicular to plane";

        case FaceRelationship::CylinderAxisParallel:
            return "Cylinder axis parallel to plane";

        case FaceRelationship::CylinderAxisOblique:
            return "Cylinder axis oblique to plane";

        case FaceRelationship::NotAnalyzed:
            return "Not analyzed";
    }

    return "Unknown";
}


// ============================================================
// Plane geometry
// ============================================================

bool getPlaneNormal(
    const TopoDS_Face& face,
    gp_Dir& normal)
{
    Handle(Geom_Surface) surface =
        BRep_Tool::Surface(face);

    Handle(Geom_Plane) plane =
        Handle(Geom_Plane)::DownCast(surface);

    if (plane.IsNull())
    {
        return false;
    }

    normal =
        plane->Pln().Axis().Direction();

    // Account for face orientation.
    if (face.Orientation() == TopAbs_REVERSED)
    {
        normal.Reverse();
    }

    return true;
}


bool getPlanePoint(
    const TopoDS_Face& face,
    gp_Pnt& point)
{
    Handle(Geom_Surface) surface =
        BRep_Tool::Surface(face);

    Handle(Geom_Plane) plane =
        Handle(Geom_Plane)::DownCast(surface);

    if (plane.IsNull())
    {
        return false;
    }

    point =
        plane->Location();

    return true;
}


// ============================================================
// Plane geometry calculations
// ============================================================

double angleBetweenNormals(
    const gp_Dir& n1,
    const gp_Dir& n2)
{
    double dot =
        n1.Dot(n2);

    // Opposite normals represent the same
    // geometric plane orientation.
    dot = std::abs(dot);

    // Protect against floating-point error.
    dot =
        std::max(
            -1.0,
            std::min(1.0, dot)
        );

    return std::acos(dot)
        * 180.0
        / M_PI;
}


bool areCoplanar(
    const gp_Pnt& p1,
    const gp_Dir& n1,
    const gp_Pnt& p2,
    double tolerance)
{
    gp_Vec displacement(
        p1,
        p2
    );

    double distance =
        std::abs(
            displacement.Dot(n1)
        );

    return distance < tolerance;
}


// ============================================================
// Plane ↔ Plane
// ============================================================

FaceRelationship classifyPlanarRelationship(
    const TopoDS_Face& face1,
    const TopoDS_Face& face2)
{
    gp_Dir normal1;
    gp_Dir normal2;

    gp_Pnt point1;
    gp_Pnt point2;

    bool isPlane1 =
        getPlaneNormal(
            face1,
            normal1
        );

    bool isPlane2 =
        getPlaneNormal(
            face2,
            normal2
        );

    bool hasPoint1 =
        getPlanePoint(
            face1,
            point1
        );

    bool hasPoint2 =
        getPlanePoint(
            face2,
            point2
        );

    if (!isPlane1 ||
        !isPlane2 ||
        !hasPoint1 ||
        !hasPoint2)
    {
        return FaceRelationship::NotAnalyzed;
    }

    double angle =
        angleBetweenNormals(
            normal1,
            normal2
        );

    constexpr double angleTolerance =
        1e-6;

    // --------------------------------------------------------
    // Parallel normals
    // --------------------------------------------------------

    if (angle < angleTolerance)
    {
        if (areCoplanar(
                point1,
                normal1,
                point2))
        {
            return FaceRelationship::Coplanar;
        }

        return FaceRelationship::Parallel;
    }

    // --------------------------------------------------------
    // Perpendicular normals
    // --------------------------------------------------------

    if (std::abs(angle - 90.0)
        < angleTolerance)
    {
        return FaceRelationship::Perpendicular;
    }

    // --------------------------------------------------------
    // General oblique relationship
    // --------------------------------------------------------

    return FaceRelationship::Oblique;
}


// ============================================================
// Surface type
// ============================================================

FaceSurfaceType getSurfaceType(
    const TopoDS_Face& face)
{
    Handle(Geom_Surface) surface =
        BRep_Tool::Surface(face);

    if (surface.IsNull())
    {
        return FaceSurfaceType::Other;
    }

    if (!Handle(Geom_Plane)::DownCast(surface).IsNull())
    {
        return FaceSurfaceType::Plane;
    }

    if (!Handle(Geom_CylindricalSurface)::DownCast(surface).IsNull())
    {
        return FaceSurfaceType::Cylinder;
    }

    return FaceSurfaceType::Other;
}


std::string surfaceTypeToString(
    FaceSurfaceType type)
{
    switch (type)
    {
        case FaceSurfaceType::Plane:
            return "Plane";

        case FaceSurfaceType::Cylinder:
            return "Cylinder";

        case FaceSurfaceType::Other:
            return "Other";
    }

    return "Unknown";
}


// ============================================================
// Cylinder geometry
// ============================================================

bool getCylinderGeometry(
    const TopoDS_Face& face,
    gp_Ax1& axis,
    double& radius)
{
    Handle(Geom_Surface) surface =
        BRep_Tool::Surface(face);

    Handle(Geom_CylindricalSurface> cylinder =
        Handle(Geom_CylindricalSurface)::DownCast(surface);

    if (cylinder.IsNull())
    {
        return false;
    }

    axis =
        cylinder->Axis();

    radius =
        cylinder->Radius();

    return true;
}


// ============================================================
// Plane ↔ Cylinder
// ============================================================
//
// We describe the orientation of the cylinder axis relative
// to the plane normal.
//
//                    Cylinder axis
//                         │
//                         │
//                         │
//                    ─────┼───── Plane
//                       normal
//
// Axis parallel to plane normal
//     → Cylinder axis perpendicular to plane
//
// Axis perpendicular to plane normal
//     → Cylinder axis parallel to plane
//
// Intermediate angle
//     → Cylinder axis oblique to plane
//
// ============================================================

FaceRelationship classifyPlaneCylinderRelationship(
    const TopoDS_Face& planeFace,
    const TopoDS_Face& cylinderFace)
{
    gp_Dir planeNormal;

    if (!getPlaneNormal(
            planeFace,
            planeNormal))
    {
        return FaceRelationship::NotAnalyzed;
    }

    gp_Ax1 cylinderAxis;
    double radius;

    if (!getCylinderGeometry(
            cylinderFace,
            cylinderAxis,
            radius))
    {
        return FaceRelationship::NotAnalyzed;
    }

    gp_Dir axisDirection =
        cylinderAxis.Direction();

    // The absolute value makes the classification
    // independent of axis direction (+n versus -n).
    double dot =
        std::abs(
            axisDirection.Dot(planeNormal)
        );

    constexpr double tolerance =
        1e-6;

    // --------------------------------------------------------
    // Axis parallel to plane normal
    //
    // Therefore:
    //
    //     cylinder axis ⟂ plane
    //
    // --------------------------------------------------------

    if (std::abs(dot - 1.0)
        < tolerance)
    {
        return FaceRelationship::CylinderAxisPerpendicular;
    }

    // --------------------------------------------------------
    // Axis perpendicular to plane normal
    //
    // Therefore:
    //
    //     cylinder axis ∥ plane
    //
    // --------------------------------------------------------

    if (dot < tolerance)
    {
        return FaceRelationship::CylinderAxisParallel;
    }

    // --------------------------------------------------------
    // Intermediate orientation
    // --------------------------------------------------------

    return FaceRelationship::CylinderAxisOblique;
}


// ============================================================
// General relationship dispatcher
// ============================================================

FaceRelationship classifyFaceRelationship(
    const TopoDS_Face& face1,
    const TopoDS_Face& face2)
{
    FaceSurfaceType type1 =
        getSurfaceType(face1);

    FaceSurfaceType type2 =
        getSurfaceType(face2);

    // --------------------------------------------------------
    // Plane ↔ Plane
    // --------------------------------------------------------

    if (type1 == FaceSurfaceType::Plane &&
        type2 == FaceSurfaceType::Plane)
    {
        return classifyPlanarRelationship(
            face1,
            face2
        );
    }

    // --------------------------------------------------------
    // Plane ↔ Cylinder
    // --------------------------------------------------------

    if (type1 == FaceSurfaceType::Plane &&
        type2 == FaceSurfaceType::Cylinder)
    {
        return classifyPlaneCylinderRelationship(
            face1,
            face2
        );
    }

    // --------------------------------------------------------
    // Cylinder ↔ Plane
    // --------------------------------------------------------

    if (type1 == FaceSurfaceType::Cylinder &&
        type2 == FaceSurfaceType::Plane)
    {
        return classifyPlaneCylinderRelationship(
            face2,
            face1
        );
    }

    // --------------------------------------------------------
    // Other combinations
    // --------------------------------------------------------

    return FaceRelationship::NotAnalyzed;
}
```

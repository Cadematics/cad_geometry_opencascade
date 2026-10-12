#ifndef FACE_RELATIONSHIPS_H
#define FACE_RELATIONSHIPS_H

#include <string>

#include <TopoDS_Face.hxx>

#include <gp_Ax1.hxx>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>


// ------------------------------------------------------------
// Face geometric relationship
// ------------------------------------------------------------

enum class FaceRelationship
{
    Coplanar,
    Parallel,
    Perpendicular,
    Oblique,
    Tangent,
    NotAnalyzed
};


std::string relationshipToString(
    FaceRelationship relationship
);


// ------------------------------------------------------------
// Face surface type
// ------------------------------------------------------------

enum class FaceSurfaceType
{
    Plane,
    Cylinder,
    Other
};


FaceSurfaceType getSurfaceType(
    const TopoDS_Face& face
);


std::string surfaceTypeToString(
    FaceSurfaceType type
);


// ------------------------------------------------------------
// Plane geometry
// ------------------------------------------------------------

bool getPlaneNormal(
    const TopoDS_Face& face,
    gp_Dir& normal
);


bool getPlanePoint(
    const TopoDS_Face& face,
    gp_Pnt& point
);


// ------------------------------------------------------------
// Plane geometry calculations
// ------------------------------------------------------------

double angleBetweenNormals(
    const gp_Dir& n1,
    const gp_Dir& n2
);


bool areCoplanar(
    const gp_Pnt& p1,
    const gp_Dir& n1,
    const gp_Pnt& p2,
    double tolerance = 1e-6
);


// ------------------------------------------------------------
// Plane ↔ Plane
// ------------------------------------------------------------

FaceRelationship classifyPlanarRelationship(
    const TopoDS_Face& face1,
    const TopoDS_Face& face2
);


// ------------------------------------------------------------
// Cylinder geometry
// ------------------------------------------------------------

bool getCylinderGeometry(
    const TopoDS_Face& face,
    gp_Ax1& axis,
    double& radius
);


// ------------------------------------------------------------
// Plane ↔ Cylinder
// ------------------------------------------------------------

FaceRelationship classifyPlaneCylinderRelationship(
    const TopoDS_Face& planeFace,
    const TopoDS_Face& cylinderFace
);


// ------------------------------------------------------------
// General relationship dispatcher
// ------------------------------------------------------------

FaceRelationship classifyFaceRelationship(
    const TopoDS_Face& face1,
    const TopoDS_Face& face2
);

#endif
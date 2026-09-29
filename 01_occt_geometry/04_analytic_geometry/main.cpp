#include <iostream>
#include <cmath>

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>

#include <gp_Lin.hxx>
#include <gp_Pln.hxx>
#include <gp_Circ.hxx>

int main()
{
    // ============================================================
    // 1. 3D Line
    // ============================================================

    gp_Pnt lineOrigin(0.0, 0.0, 0.0);
    gp_Dir lineDirection(1.0, 0.0, 0.0);

    gp_Lin line(lineOrigin, lineDirection);

    std::cout << "3D Line:"
              << std::endl;

    std::cout << "Origin: "
              << line.Location().X() << ", "
              << line.Location().Y() << ", "
              << line.Location().Z()
              << std::endl;

    std::cout << "Direction: "
              << line.Direction().X() << ", "
              << line.Direction().Y() << ", "
              << line.Direction().Z()
              << std::endl;


    // ============================================================
    // 2. Evaluate a point on the line
    //
    // P(u) = P0 + u * D
    // ============================================================

    double parameter = 5.0;

    gp_Vec lineDisplacement(line.Direction());
    lineDisplacement *= parameter;

    gp_Pnt pointOnLine =
        line.Location().Translated(lineDisplacement);

    std::cout << "\nPoint on line:"
              << std::endl;

    std::cout << "Parameter: "
              << parameter
              << std::endl;

    std::cout << "Point: "
              << pointOnLine.X() << ", "
              << pointOnLine.Y() << ", "
              << pointOnLine.Z()
              << std::endl;


    // ============================================================
    // 3. Distance from a point to a line
    // ============================================================

    gp_Pnt testPoint(3.0, 4.0, 0.0);

    double lineDistance =
        line.Distance(testPoint);

    std::cout << "\nPoint-to-line distance:"
              << std::endl;

    std::cout << "Point: "
              << testPoint.X() << ", "
              << testPoint.Y() << ", "
              << testPoint.Z()
              << std::endl;

    std::cout << "Distance: "
              << lineDistance
              << std::endl;


    // ============================================================
    // 4. Plane
    // ============================================================

    gp_Pnt planeOrigin(0.0, 0.0, 0.0);
    gp_Dir planeNormal(0.0, 0.0, 1.0);

    gp_Pln plane(
        planeOrigin,
        planeNormal
    );

    std::cout << "\nPlane:"
              << std::endl;

    std::cout << "Origin: "
              << plane.Location().X() << ", "
              << plane.Location().Y() << ", "
              << plane.Location().Z()
              << std::endl;

    std::cout << "Normal: "
              << plane.Axis().Direction().X() << ", "
              << plane.Axis().Direction().Y() << ", "
              << plane.Axis().Direction().Z()
              << std::endl;


    // ============================================================
    // 5. Distance from a point to a plane
    // ============================================================

    gp_Pnt pointAbovePlane(0.0, 0.0, 10.0);

    double planeDistance =
        plane.Distance(pointAbovePlane);

    std::cout << "\nPoint-to-plane distance:"
              << std::endl;

    std::cout << "Point: "
              << pointAbovePlane.X() << ", "
              << pointAbovePlane.Y() << ", "
              << pointAbovePlane.Z()
              << std::endl;

    std::cout << "Distance: "
              << planeDistance
              << std::endl;


    // ============================================================
    // 6. Project a point onto the XY plane
    //
    // Plane equation:
    //
    // z = 0
    //
    // Therefore:
    //
    // (x, y, z) -> (x, y, 0)
    // ============================================================

    gp_Pnt projectedPoint(
        pointAbovePlane.X(),
        pointAbovePlane.Y(),
        0.0
    );

    std::cout << "\nPoint projection onto plane:"
              << std::endl;

    std::cout << "Original point: "
              << pointAbovePlane.X() << ", "
              << pointAbovePlane.Y() << ", "
              << pointAbovePlane.Z()
              << std::endl;

    std::cout << "Projected point: "
              << projectedPoint.X() << ", "
              << projectedPoint.Y() << ", "
              << projectedPoint.Z()
              << std::endl;


    // ============================================================
    // 7. Circle
    // ============================================================

    gp_Pnt circleCenter(0.0, 0.0, 0.0);

    gp_Dir circleNormal(0.0, 0.0, 1.0);

    gp_Ax2 circleAxis(
        circleCenter,
        circleNormal
    );

    double radius = 5.0;

    gp_Circ circle(
        circleAxis,
        radius
    );

    std::cout << "\nCircle:"
              << std::endl;

    std::cout << "Center: "
              << circle.Location().X() << ", "
              << circle.Location().Y() << ", "
              << circle.Location().Z()
              << std::endl;

    std::cout << "Radius: "
              << circle.Radius()
              << std::endl;

    std::cout << "Normal: "
              << circle.Axis().Direction().X() << ", "
              << circle.Axis().Direction().Y() << ", "
              << circle.Axis().Direction().Z()
              << std::endl;


    // ============================================================
    // 8. Evaluate points on the circle
    //
    // P(u) = C + r*cos(u)*X + r*sin(u)*Y
    // ============================================================

    const double pi = 3.14159265358979323846;

    double angle0 = 0.0;
    double angle90 = pi / 2.0;
    double angle180 = pi;

    gp_Dir circleX =
        circle.Position().XDirection();

    gp_Dir circleY =
        circle.Position().YDirection();

    gp_Vec xComponent(circleX);
    gp_Vec yComponent(circleY);

    gp_Pnt circlePoint0 = circleCenter;

    gp_Pnt circlePoint90 =
        circleCenter.Translated(
            yComponent * radius
        );

    gp_Pnt circlePoint180 =
        circleCenter.Translated(
            xComponent * (-radius)
        );

    std::cout << "\nPoints on circle:"
              << std::endl;

    std::cout << "0 degrees: "
              << circlePoint0.X() + radius << ", "
              << circlePoint0.Y() << ", "
              << circlePoint0.Z()
              << std::endl;

    std::cout << "90 degrees: "
              << circlePoint90.X() << ", "
              << circlePoint90.Y() << ", "
              << circlePoint90.Z()
              << std::endl;

    std::cout << "180 degrees: "
              << circlePoint180.X() << ", "
              << circlePoint180.Y() << ", "
              << circlePoint180.Z()
              << std::endl;


    // ============================================================
    // 9. Distance from circle center to a point on circle
    // ============================================================

    gp_Pnt actualCirclePoint0(
        radius,
        0.0,
        0.0
    );

    double radiusCheck =
        circle.Location().Distance(actualCirclePoint0);

    std::cout << "\nCircle radius verification:"
              << std::endl;

    std::cout << "Expected radius: "
              << circle.Radius()
              << std::endl;

    std::cout << "Measured radius: "
              << radiusCheck
              << std::endl;


   // ============================================================
// 10. Line / plane relationship
//
// For a line with direction D and plane normal N:
//
// D · N = 0  -> line is parallel to plane
// |D · N| = 1 -> line is perpendicular to plane
// otherwise  -> line is oblique and intersects plane
// ============================================================

const double tolerance = 1e-6;

gp_Dir planeNormalForRelation =
    plane.Axis().Direction();


// ------------------------------------------------------------
// Case 1: perpendicular to plane
// ------------------------------------------------------------

gp_Pnt perpendicularOrigin(0.0, 0.0, 10.0);
gp_Dir perpendicularDirection(0.0, 0.0, 1.0);

gp_Lin perpendicularLine(
    perpendicularOrigin,
    perpendicularDirection
);

double perpendicularDot =
    perpendicularLine.Direction().Dot(
        planeNormalForRelation
    );

std::cout << "\nLine / plane relationship:"
          << std::endl;

std::cout << "Case 1 - Perpendicular:"
          << std::endl;

std::cout << "Direction dot normal: "
          << perpendicularDot
          << std::endl;

if (std::abs(std::abs(perpendicularDot) - 1.0) < tolerance)
{
    std::cout << "Relationship: perpendicular"
              << std::endl;
}


// ------------------------------------------------------------
// Case 2: parallel to plane
// ------------------------------------------------------------

gp_Pnt parallelOrigin(0.0, 0.0, 10.0);
gp_Dir parallelDirection(1.0, 0.0, 0.0);

gp_Lin parallelLine(
    parallelOrigin,
    parallelDirection
);

double parallelDot =
    parallelLine.Direction().Dot(
        planeNormalForRelation
    );

std::cout << "\nCase 2 - Parallel:"
          << std::endl;

std::cout << "Direction dot normal: "
          << parallelDot
          << std::endl;

if (std::abs(parallelDot) < tolerance)
{
    std::cout << "Relationship: parallel"
              << std::endl;
}


// ------------------------------------------------------------
// Case 3: line intersects plane
// ------------------------------------------------------------

gp_Pnt intersectOrigin(0.0, 0.0, 10.0);
gp_Dir intersectDirection(0.0, 0.0, -1.0);

gp_Lin intersectingLine(
    intersectOrigin,
    intersectDirection
);

double intersectionParameter = 10.0;

gp_Vec intersectionDisplacement(
    intersectingLine.Direction()
);

intersectionDisplacement *=
    intersectionParameter;

gp_Pnt intersectionPoint =
    intersectingLine.Location().Translated(
        intersectionDisplacement
    );

std::cout << "\nCase 3 - Intersecting:"
          << std::endl;

std::cout << "Intersection point: "
          << intersectionPoint.X() << ", "
          << intersectionPoint.Y() << ", "
          << intersectionPoint.Z()
          << std::endl;

if (std::abs(intersectionPoint.Z()) < tolerance)
{
    std::cout << "Relationship: intersects plane"
              << std::endl;
}

    // ============================================================
    // 11. Geometric verification
    // ============================================================

    // const double tolerance = 1e-6;

    bool radiusIsCorrect =
        std::abs(radiusCheck - radius) < tolerance;

    bool projectionIsOnPlane =
        std::abs(projectedPoint.Z()) < tolerance;

    std::cout << "\nVerification:"
              << std::endl;

    std::cout << "Circle radius correct: "
              << radiusIsCorrect
              << std::endl;

    std::cout << "Projected point lies on plane: "
              << projectionIsOnPlane
              << std::endl;


    return 0;
}
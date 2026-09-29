#include <iostream>
#include <gp_Ax3.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Trsf.hxx>

int main()
{
    // ============================================================
    // 1. Original point
    // ============================================================

    gp_Pnt point(1.0, 2.0, 3.0);

    std::cout << "Original point: "
              << point.X() << ", "
              << point.Y() << ", "
              << point.Z()
              << std::endl;


    // ============================================================
    // 2. Translation
    // ============================================================

    gp_Vec translation(10.0, 20.0, 30.0);

    gp_Trsf translationTrsf;
    translationTrsf.SetTranslation(translation);

    gp_Pnt translatedPoint = point.Transformed(translationTrsf);

    std::cout << "\nTranslation:"
              << std::endl;

    std::cout << "Translation vector: "
              << translation.X() << ", "
              << translation.Y() << ", "
              << translation.Z()
              << std::endl;

    std::cout << "Translated point: "
              << translatedPoint.X() << ", "
              << translatedPoint.Y() << ", "
              << translatedPoint.Z()
              << std::endl;


    // ============================================================
    // 3. Rotation around the Z axis
    // ============================================================

    gp_Pnt rotationOrigin(0.0, 0.0, 0.0);

    gp_Dir zDirection(0.0, 0.0, 1.0);

    gp_Ax1 zAxis(rotationOrigin, zDirection);

    const double angle = 3.14159265358979323846 / 2.0;

    gp_Trsf rotationTrsf;
    rotationTrsf.SetRotation(zAxis, angle);

    gp_Pnt pointToRotate(1.0, 0.0, 0.0);

    gp_Pnt rotatedPoint = pointToRotate.Transformed(rotationTrsf);

    std::cout << "\nRotation:"
              << std::endl;

    std::cout << "Original point: "
              << pointToRotate.X() << ", "
              << pointToRotate.Y() << ", "
              << pointToRotate.Z()
              << std::endl;

    std::cout << "Rotation angle: 90 degrees"
              << std::endl;

    std::cout << "Rotated point: "
              << rotatedPoint.X() << ", "
              << rotatedPoint.Y() << ", "
              << rotatedPoint.Z()
              << std::endl;


    // ============================================================
    // 4. Transform a direction
    // ============================================================

    gp_Dir direction(1.0, 0.0, 0.0);

    gp_Dir rotatedDirection = direction.Transformed(rotationTrsf);

    std::cout << "\nDirection rotation:"
              << std::endl;

    std::cout << "Original direction: "
              << direction.X() << ", "
              << direction.Y() << ", "
              << direction.Z()
              << std::endl;

    std::cout << "Rotated direction: "
              << rotatedDirection.X() << ", "
              << rotatedDirection.Y() << ", "
              << rotatedDirection.Z()
              << std::endl;


    // ============================================================
    // 5. Rotation around a custom axis
    // ============================================================

    gp_Pnt customAxisOrigin(0.0, 0.0, 0.0);

    gp_Dir customAxisDirection(0.0, 1.0, 0.0);

    gp_Ax1 yAxis(
        customAxisOrigin,
        customAxisDirection
    );

    gp_Trsf yRotation;

    yRotation.SetRotation(
        yAxis,
        angle
    );

    gp_Pnt xPoint(1.0, 0.0, 0.0);

    gp_Pnt rotatedAroundY =
        xPoint.Transformed(yRotation);

    std::cout << "\nRotation around Y axis:"
              << std::endl;

    std::cout << "Original point: "
              << xPoint.X() << ", "
              << xPoint.Y() << ", "
              << xPoint.Z()
              << std::endl;

    std::cout << "Rotated point: "
              << rotatedAroundY.X() << ", "
              << rotatedAroundY.Y() << ", "
              << rotatedAroundY.Z()
              << std::endl;


    // ============================================================
    // 6. Combine translation and rotation
    // ============================================================

    gp_Trsf combinedTrsf;

    combinedTrsf.SetRotation(
        zAxis,
        angle
    );

    combinedTrsf.SetTranslationPart(
        gp_Vec(10.0, 0.0, 0.0)
    );

    gp_Pnt combinedPoint(1.0, 0.0, 0.0);

    gp_Pnt transformedCombined =
        combinedPoint.Transformed(combinedTrsf);

    std::cout << "\nCombined transformation:"
              << std::endl;

    std::cout << "Original point: "
              << combinedPoint.X() << ", "
              << combinedPoint.Y() << ", "
              << combinedPoint.Z()
              << std::endl;

    std::cout << "Transformed point: "
              << transformedCombined.X() << ", "
              << transformedCombined.Y() << ", "
              << transformedCombined.Z()
              << std::endl;


    // ============================================================
    // 7. Coordinate system transformation
    // ============================================================

    gp_Pnt localOrigin(100.0, 50.0, 25.0);

    gp_Dir localZ(0.0, 0.0, 1.0);
    gp_Dir localX(1.0, 0.0, 0.0);

    gp_Ax3 localFrame(
        localOrigin,
        localZ,
        localX
    );

    gp_Trsf frameTrsf;

    frameTrsf.SetTransformation(
        localFrame,
        gp_Ax3()
    );

    gp_Pnt localPoint(1.0, 2.0, 3.0);

    gp_Pnt globalPoint =
        localPoint.Transformed(frameTrsf);

    std::cout << "\nCoordinate-system transformation:"
              << std::endl;

    std::cout << "Local point: "
              << localPoint.X() << ", "
              << localPoint.Y() << ", "
              << localPoint.Z()
              << std::endl;

    std::cout << "Global point: "
              << globalPoint.X() << ", "
              << globalPoint.Y() << ", "
              << globalPoint.Z()
              << std::endl;


    // ============================================================
    // 8. Verify distance is preserved by rigid transformation
    // ============================================================

    gp_Pnt p1(1.0, 2.0, 3.0);
    gp_Pnt p2(4.0, 6.0, 3.0);

    double originalDistance = p1.Distance(p2);

    gp_Pnt transformedP1 =
        p1.Transformed(rotationTrsf);

    gp_Pnt transformedP2 =
        p2.Transformed(rotationTrsf);

    double transformedDistance =
        transformedP1.Distance(transformedP2);

    std::cout << "\nDistance preservation:"
              << std::endl;

    std::cout << "Original distance: "
              << originalDistance
              << std::endl;

    std::cout << "Transformed distance: "
              << transformedDistance
              << std::endl;


    return 0;
}
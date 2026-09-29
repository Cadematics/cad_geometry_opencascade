#include <iostream>

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>

int main()
{
    // ============================================================
    // 1. Create the origin and the three principal directions
    // ============================================================

    gp_Pnt origin(0.0, 0.0, 0.0);

    gp_Dir xDir(1.0, 0.0, 0.0);
    gp_Dir yDir(0.0, 1.0, 0.0);
    gp_Dir zDir(0.0, 0.0, 1.0);

    std::cout << "Origin: "
              << origin.X() << ", "
              << origin.Y() << ", "
              << origin.Z()
              << std::endl;


    // ============================================================
    // 2. Create three axes
    // ============================================================

    gp_Ax1 xAxis(origin, xDir);
    gp_Ax1 yAxis(origin, yDir);
    gp_Ax1 zAxis(origin, zDir);

    std::cout << "\nPrincipal axes created."
              << std::endl;

    std::cout << "X axis direction: "
              << xAxis.Direction().X() << ", "
              << xAxis.Direction().Y() << ", "
              << xAxis.Direction().Z()
              << std::endl;

    std::cout << "Y axis direction: "
              << yAxis.Direction().X() << ", "
              << yAxis.Direction().Y() << ", "
              << yAxis.Direction().Z()
              << std::endl;

    std::cout << "Z axis direction: "
              << zAxis.Direction().X() << ", "
              << zAxis.Direction().Y() << ", "
              << zAxis.Direction().Z()
              << std::endl;


    // ============================================================
    // 3. Create a custom axis
    // ============================================================

    gp_Pnt axisOrigin(10.0, 20.0, 30.0);

    gp_Dir axisDirection(1.0, 1.0, 0.0);

    gp_Ax1 customAxis(axisOrigin, axisDirection);

    std::cout << "\nCustom axis:"
              << std::endl;

    std::cout << "Origin: "
              << customAxis.Location().X() << ", "
              << customAxis.Location().Y() << ", "
              << customAxis.Location().Z()
              << std::endl;

    std::cout << "Direction: "
              << customAxis.Direction().X() << ", "
              << customAxis.Direction().Y() << ", "
              << customAxis.Direction().Z()
              << std::endl;


    // ============================================================
    // 4. Create a 3D coordinate system with gp_Ax2
    // ============================================================

    gp_Ax2 coordinateSystem(
        origin,
        zDir,
        xDir
    );

    std::cout << "\nCoordinate system (gp_Ax2):"
              << std::endl;

    std::cout << "Origin: "
              << coordinateSystem.Location().X() << ", "
              << coordinateSystem.Location().Y() << ", "
              << coordinateSystem.Location().Z()
              << std::endl;

    std::cout << "Main direction (Z): "
              << coordinateSystem.Direction().X() << ", "
              << coordinateSystem.Direction().Y() << ", "
              << coordinateSystem.Direction().Z()
              << std::endl;

    std::cout << "X direction: "
              << coordinateSystem.XDirection().X() << ", "
              << coordinateSystem.XDirection().Y() << ", "
              << coordinateSystem.XDirection().Z()
              << std::endl;

    std::cout << "Y direction: "
              << coordinateSystem.YDirection().X() << ", "
              << coordinateSystem.YDirection().Y() << ", "
              << coordinateSystem.YDirection().Z()
              << std::endl;


// ============================================================
// 5. Access the directions of gp_Ax2
// ============================================================

gp_Dir xSystemDirection = coordinateSystem.XDirection();
gp_Dir ySystemDirection = coordinateSystem.YDirection();

std::cout << "\nDirections belonging to the coordinate system:"
          << std::endl;

std::cout << "Main axis direction: "
          << coordinateSystem.Direction().X() << ", "
          << coordinateSystem.Direction().Y() << ", "
          << coordinateSystem.Direction().Z()
          << std::endl;

std::cout << "X direction: "
          << xSystemDirection.X() << ", "
          << xSystemDirection.Y() << ", "
          << xSystemDirection.Z()
          << std::endl;

std::cout << "Y direction: "
          << ySystemDirection.X() << ", "
          << ySystemDirection.Y() << ", "
          << ySystemDirection.Z()
          << std::endl;


    // ============================================================
    // 6. Create a gp_Ax3 coordinate system
    // ============================================================

    gp_Ax3 referenceFrame(
        origin,
        zDir,
        xDir
    );

    std::cout << "\nReference frame (gp_Ax3):"
              << std::endl;

    std::cout << "Origin: "
              << referenceFrame.Location().X() << ", "
              << referenceFrame.Location().Y() << ", "
              << referenceFrame.Location().Z()
              << std::endl;

    std::cout << "X direction: "
              << referenceFrame.XDirection().X() << ", "
              << referenceFrame.XDirection().Y() << ", "
              << referenceFrame.XDirection().Z()
              << std::endl;

    std::cout << "Y direction: "
              << referenceFrame.YDirection().X() << ", "
              << referenceFrame.YDirection().Y() << ", "
              << referenceFrame.YDirection().Z()
              << std::endl;

    std::cout << "Z direction: "
              << referenceFrame.Direction().X() << ", "
              << referenceFrame.Direction().Y() << ", "
              << referenceFrame.Direction().Z()
              << std::endl;


    // ============================================================
    // 7. Create a local coordinate system away from the origin
    // ============================================================

    gp_Pnt localOrigin(100.0, 50.0, 25.0);

    gp_Dir localZ(0.0, 0.0, 1.0);
    gp_Dir localX(1.0, 0.0, 0.0);

    gp_Ax3 localFrame(
        localOrigin,
        localZ,
        localX
    );

    std::cout << "\nLocal coordinate system:"
              << std::endl;

    std::cout << "Origin: "
              << localFrame.Location().X() << ", "
              << localFrame.Location().Y() << ", "
              << localFrame.Location().Z()
              << std::endl;

    std::cout << "X direction: "
              << localFrame.XDirection().X() << ", "
              << localFrame.XDirection().Y() << ", "
              << localFrame.XDirection().Z()
              << std::endl;

    std::cout << "Y direction: "
              << localFrame.YDirection().X() << ", "
              << localFrame.YDirection().Y() << ", "
              << localFrame.YDirection().Z()
              << std::endl;

    std::cout << "Z direction: "
              << localFrame.Direction().X() << ", "
              << localFrame.Direction().Y() << ", "
              << localFrame.Direction().Z()
              << std::endl;


    // ============================================================
    // 8. Check coordinate-system relationships
    // ============================================================

    std::cout << "\nCoordinate-system checks:"
              << std::endl;

    std::cout << "X perpendicular to Y: "
              << xDir.IsNormal(yDir, 1e-6 )
              << std::endl;

    std::cout << "X perpendicular to Z: "
              << xDir.IsNormal(zDir, 1e-6)
              << std::endl;

    std::cout << "Y perpendicular to Z: "
              << yDir.IsNormal(zDir, 1e-6)
              << std::endl;


    // ============================================================
    // 9. Compare two coordinate systems
    // ============================================================

    gp_Ax3 anotherFrame(
        origin,
        zDir,
        xDir
    );

    std::cout << "\nFrame comparison:"
              << std::endl;

    std::cout << "Same location: "
              << referenceFrame.Location().IsEqual(
                     anotherFrame.Location(),
                     1e-6
                 )
              << std::endl;

    std::cout << "Same X direction: "
              << referenceFrame.XDirection().IsEqual(
                     anotherFrame.XDirection(),
                     1e-6
                 )
              << std::endl;

    std::cout << "Same Z direction: "
              << referenceFrame.Direction().IsEqual(
                     anotherFrame.Direction(),
                     1e-6
                 )
              << std::endl;


    return 0;
}
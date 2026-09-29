#include <iostream>

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>

int main()
{
    // ============================================================
    // 1. Point
    // ============================================================

    gp_Pnt p(10.0, 20.0, 30.0);

    std::cout << "Point: "
              << p.X() << ", "
              << p.Y() << ", "
              << p.Z() << std::endl;


    // ============================================================
    // 2. Vector
    // ============================================================

    gp_Vec v(3.0, 4.0, 0.0);

    std::cout << "Vector: "
              << v.X() << ", "
              << v.Y() << ", "
              << v.Z() << std::endl;

    std::cout << "Vector magnitude: "
              << v.Magnitude() << std::endl;


    // ============================================================
    // 3. Direction
    // ============================================================

    // gp_Dir automatically normalizes the vector.
    gp_Dir d(v);

    std::cout << "Direction: "
              << d.X() << ", "
              << d.Y() << ", "
              << d.Z() << std::endl;


    // ============================================================
    // 4. Vector between two points
    // ============================================================

    gp_Pnt p1(1.0, 2.0, 3.0);
    gp_Pnt p2(4.0, 6.0, 3.0);

    gp_Vec displacement(p1, p2);

    std::cout << "\nDisplacement:" << std::endl;

    std::cout << "dx = "
              << displacement.X() << std::endl;

    std::cout << "dy = "
              << displacement.Y() << std::endl;

    std::cout << "dz = "
              << displacement.Z() << std::endl;

    std::cout << "Magnitude = "
              << displacement.Magnitude() << std::endl;


    // ============================================================
    // 5. Distance between two points
    // ============================================================

    std::cout << "Distance = "
              << p1.Distance(p2)
              << std::endl;


    // ============================================================
    // 6. Dot product
    // ============================================================

    gp_Vec a(1.0, 0.0, 0.0);
    gp_Vec b(0.0, 1.0, 0.0);

    double dot = a.Dot(b);

    std::cout << "\nDot product:" << std::endl;
    std::cout << "a · b = "
              << dot
              << std::endl;


    // ============================================================
    // 7. Cross product
    // ============================================================

    gp_Vec cross = a.Crossed(b);

    std::cout << "\nCross product:" << std::endl;

    std::cout << "a x b = "
              << cross.X() << ", "
              << cross.Y() << ", "
              << cross.Z()
              << std::endl;


    // ============================================================
    // 8. Point + vector
    // ============================================================

    gp_Pnt p3(1.0, 2.0, 3.0);
    gp_Vec translation(10.0, 20.0, 30.0);

    gp_Pnt p4 = p3.Translated(translation);

    std::cout << "\nPoint + vector:" << std::endl;

    std::cout << "Original point: "
              << p3.X() << ", "
              << p3.Y() << ", "
              << p3.Z()
              << std::endl;

    std::cout << "Translated point: "
              << p4.X() << ", "
              << p4.Y() << ", "
              << p4.Z()
              << std::endl;


    return 0;
}
#include <iostream>


#include <gp_Pln.hxx>
#include <gp_Ax3.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

#include <STEPControl_Reader.hxx>
#include <IFSelect_ReturnStatus.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>

#include <TopExp_Explorer.hxx>

#include <BRep_Tool.hxx>

#include <Geom_Surface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_SphericalSurface.hxx>
#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>


int main(int argc, char* argv[])
{
    // ============================================================
    // 1. Check command-line argument
    // ============================================================

    if (argc != 2)
    {
        std::cerr << "Usage: "
                  << argv[0]
                  << " <step_file>"
                  << std::endl;

        return 1;
    }

    const char* filename = argv[1];


    // ============================================================
    // 2. Read STEP file
    // ============================================================

    STEPControl_Reader reader;

    IFSelect_ReturnStatus status =
        reader.ReadFile(filename);

    if (status != IFSelect_RetDone)
    {
        std::cerr << "ERROR: Could not read STEP file."
                  << std::endl;

        return 1;
    }

    if (!reader.TransferRoots())
    {
        std::cerr << "ERROR: Could not transfer STEP data."
                  << std::endl;

        return 1;
    }

    TopoDS_Shape shape =
        reader.OneShape();

    if (shape.IsNull())
    {
        std::cerr << "ERROR: Shape is null."
                  << std::endl;

        return 1;
    }


    // ============================================================
    // 3. Traverse faces
    // ============================================================

    std::cout << "Geometry extraction"
              << std::endl;

    std::cout << "==================="
              << std::endl;

    std::cout << "File: "
              << filename
              << std::endl;


    int faceIndex = 0;

    for (TopExp_Explorer explorer(
             shape,
             TopAbs_FACE);
         explorer.More();
         explorer.Next())
    {
        TopoDS_Face face =
            TopoDS::Face(
                explorer.Current()
            );

        faceIndex++;

        Handle(Geom_Surface) surface =
            BRep_Tool::Surface(face);

        std::cout << "\nFace "
                  << faceIndex
                  << std::endl;

        std::cout << "-----"
                  << std::endl;


        if (surface.IsNull())
        {
            std::cout << "Surface: NULL"
                      << std::endl;

            continue;
        }


        // ========================================================
        // 4. Identify surface type
        // ========================================================

        if (surface->IsKind(
                STANDARD_TYPE(Geom_Plane)))
        {
            Handle(Geom_Plane) plane =
                Handle(Geom_Plane)::DownCast(
                    surface
                );

            std::cout << "Surface type: Plane"
                      << std::endl;

            gp_Pln geometry =
                plane->Pln();

            gp_Pnt location =
                geometry.Location();

            gp_Dir normal =
                geometry.Axis().Direction();

            std::cout << "Origin: "
                      << location.X()
                      << ", "
                      << location.Y()
                      << ", "
                      << location.Z()
                      << std::endl;

            std::cout << "Normal: "
                      << normal.X()
                      << ", "
                      << normal.Y()
                      << ", "
                      << normal.Z()
                      << std::endl;
        }


        else if (surface->IsKind(
                     STANDARD_TYPE(
                         Geom_CylindricalSurface)))
        {
            Handle(Geom_CylindricalSurface) cylinder =
                Handle(Geom_CylindricalSurface)::DownCast(
                    surface
                );

            std::cout << "Surface type: Cylinder"
                      << std::endl;

            std::cout << "Radius: "
                      << cylinder->Radius()
                      << std::endl;

            gp_Ax3 axis =
                cylinder->Position();

            gp_Pnt location =
                axis.Location();

            gp_Dir direction =
                axis.Direction();

            std::cout << "Axis origin: "
                      << location.X()
                      << ", "
                      << location.Y()
                      << ", "
                      << location.Z()
                      << std::endl;

            std::cout << "Axis direction: "
                      << direction.X()
                      << ", "
                      << direction.Y()
                      << ", "
                      << direction.Z()
                      << std::endl;
        }


        else if (surface->IsKind(
                     STANDARD_TYPE(
                         Geom_SphericalSurface)))
        {
            Handle(Geom_SphericalSurface) sphere =
                Handle(Geom_SphericalSurface)::DownCast(
                    surface
                );

            std::cout << "Surface type: Sphere"
                      << std::endl;

            std::cout << "Radius: "
                      << sphere->Radius()
                      << std::endl;
        }


        else if (surface->IsKind(
                     STANDARD_TYPE(
                         Geom_ConicalSurface)))
        {
            Handle(Geom_ConicalSurface) cone =
                Handle(Geom_ConicalSurface)::DownCast(
                    surface
                );

            std::cout << "Surface type: Cone"
                      << std::endl;

            std::cout << "Semi-angle: "
                      << cone->SemiAngle()
                      << std::endl;

            std::cout << "Reference radius: "
                      << cone->RefRadius()
                      << std::endl;
        }


        else if (surface->IsKind(
                     STANDARD_TYPE(
                         Geom_ToroidalSurface)))
        {
            Handle(Geom_ToroidalSurface) torus =
                Handle(Geom_ToroidalSurface)::DownCast(
                    surface
                );

            std::cout << "Surface type: Torus"
                      << std::endl;

            std::cout << "Major radius: "
                      << torus->MajorRadius()
                      << std::endl;

            std::cout << "Minor radius: "
                      << torus->MinorRadius()
                      << std::endl;
        }


        else
        {
            std::cout << "Surface type: Other"
                      << std::endl;

            std::cout << "OCCT type: "
                      << surface->DynamicType()->Name()
                      << std::endl;
        }
    }


    return 0;
}
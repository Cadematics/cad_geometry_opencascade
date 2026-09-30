
#include <iostream>
#include <string>
#include <vector>

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Pln.hxx>

#include <STEPControl_Reader.hxx>
#include <IFSelect_ReturnStatus.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>

#include <TopExp_Explorer.hxx>

#include <BRep_Tool.hxx>
#include <BRepGProp.hxx>

#include <GProp_GProps.hxx>

#include <Geom_Surface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_SphericalSurface.hxx>
#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>


struct FaceInfo
{
    int id;
    std::string surface_type;
    double area;
    gp_Pnt centroid;
    gp_Dir normal;
    int edge_count;
};


std::string surfaceType(const TopoDS_Face& face)
{
    Handle(Geom_Surface) surface = BRep_Tool::Surface(face);

    if (surface.IsNull())
        return "Unknown";

    if (surface->IsKind(STANDARD_TYPE(Geom_Plane)))
        return "Plane";

    if (surface->IsKind(STANDARD_TYPE(Geom_CylindricalSurface)))
        return "Cylinder";

    if (surface->IsKind(STANDARD_TYPE(Geom_SphericalSurface)))
        return "Sphere";

    if (surface->IsKind(STANDARD_TYPE(Geom_ConicalSurface)))
        return "Cone";

    if (surface->IsKind(STANDARD_TYPE(Geom_ToroidalSurface)))
        return "Torus";

    return surface->DynamicType()->Name();
}


FaceInfo inspectFace(const TopoDS_Face& face, int id)
{
    FaceInfo info;

    info.id = id;
    info.surface_type = surfaceType(face);

    // ------------------------------------------------------------
    // Surface area and centroid
    // ------------------------------------------------------------

    GProp_GProps props;

    BRepGProp::SurfaceProperties(face, props);

    info.area = props.Mass();
    info.centroid = props.CentreOfMass();


    // ------------------------------------------------------------
    // Face normal
    //
    // For this first version we only extract the normal for
    // planar faces.
    // ------------------------------------------------------------

    info.normal = gp_Dir(1, 0, 0);

    Handle(Geom_Surface) surface = BRep_Tool::Surface(face);

    if (!surface.IsNull() &&
        surface->IsKind(STANDARD_TYPE(Geom_Plane)))
    {
        Handle(Geom_Plane) plane =
            Handle(Geom_Plane)::DownCast(surface);

        info.normal = plane->Pln().Axis().Direction();
    }


    // ------------------------------------------------------------
    // Count edges
    // ------------------------------------------------------------

    info.edge_count = 0;

    for (TopExp_Explorer explorer(face, TopAbs_EDGE);
         explorer.More();
         explorer.Next())
    {
        ++info.edge_count;
    }


    return info;
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: "
                  << argv[0]
                  << " <file.step>\n";

        return 1;
    }

    std::string filename = argv[1];

    std::cout << "B-REP INSPECTOR\n";
    std::cout << "===============\n\n";

    std::cout << "File: "
              << filename
              << "\n\n";


    // ------------------------------------------------------------
    // Read STEP file
    // ------------------------------------------------------------

    STEPControl_Reader reader;

    IFSelect_ReturnStatus status =
        reader.ReadFile(filename.c_str());

    if (status != IFSelect_RetDone)
    {
        std::cerr << "Error: could not read STEP file.\n";
        return 1;
    }

    if (!reader.TransferRoots())
    {
        std::cerr << "Error: could not transfer STEP roots.\n";
        return 1;
    }

    TopoDS_Shape shape = reader.OneShape();

    if (shape.IsNull())
    {
        std::cerr << "Error: resulting shape is null.\n";
        return 1;
    }


    // ------------------------------------------------------------
    // Model topology
    // ------------------------------------------------------------

    int solid_count = 0;
    int shell_count = 0;
    int face_count = 0;
    int edge_count = 0;
    int vertex_count = 0;

    for (TopExp_Explorer explorer(shape, TopAbs_SOLID);
         explorer.More();
         explorer.Next())
    {
        ++solid_count;
    }

    for (TopExp_Explorer explorer(shape, TopAbs_SHELL);
         explorer.More();
         explorer.Next())
    {
        ++shell_count;
    }

    for (TopExp_Explorer explorer(shape, TopAbs_FACE);
         explorer.More();
         explorer.Next())
    {
        ++face_count;
    }

    for (TopExp_Explorer explorer(shape, TopAbs_EDGE);
         explorer.More();
         explorer.Next())
    {
        ++edge_count;
    }

    for (TopExp_Explorer explorer(shape, TopAbs_VERTEX);
         explorer.More();
         explorer.Next())
    {
        ++vertex_count;
    }


    std::cout << "MODEL\n";
    std::cout << "-----\n";

    std::cout << "Solids:   "
              << solid_count << '\n';

    std::cout << "Shells:   "
              << shell_count << '\n';

    std::cout << "Faces:    "
              << face_count << '\n';

    std::cout << "Edges:    "
              << edge_count << '\n';

    std::cout << "Vertices: "
              << vertex_count << '\n';


    // ------------------------------------------------------------
    // Face inspection
    // ------------------------------------------------------------

    std::cout << "\nFACES\n";
    std::cout << "-----\n";

    int face_id = 1;

    for (TopExp_Explorer explorer(shape, TopAbs_FACE);
         explorer.More();
         explorer.Next())
    {
        TopoDS_Face face =
            TopoDS::Face(explorer.Current());

        FaceInfo info =
            inspectFace(face, face_id);


        std::cout << "\nFace "
                  << info.id
                  << '\n';

        std::cout << "------\n";

        std::cout << "Surface: "
                  << info.surface_type
                  << '\n';

        std::cout << "Area: "
                  << info.area
                  << '\n';

        std::cout << "Centroid: "
                  << info.centroid.X()
                  << ", "
                  << info.centroid.Y()
                  << ", "
                  << info.centroid.Z()
                  << '\n';

        if (info.surface_type == "Plane")
        {
            std::cout << "Normal: "
                      << info.normal.X()
                      << ", "
                      << info.normal.Y()
                      << ", "
                      << info.normal.Z()
                      << '\n';
        }

        std::cout << "Edges: "
                  << info.edge_count
                  << '\n';

        ++face_id;
    }

    return 0;
}

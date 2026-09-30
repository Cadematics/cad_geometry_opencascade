#include <iostream>

#include <gp_Pnt.hxx>

#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <BRep_Tool.hxx>

#include <TopExp_Explorer.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>

#include <Geom_Surface.hxx>


int main()
{
    // ============================================================
    // 1. Define rectangle corners
    // ============================================================

    gp_Pnt p1(0.0, 0.0, 0.0);
    gp_Pnt p2(10.0, 0.0, 0.0);
    gp_Pnt p3(10.0, 20.0, 0.0);
    gp_Pnt p4(0.0, 20.0, 0.0);

    std::cout << "Rectangle:"
              << std::endl;

    std::cout << "P1: "
              << p1.X() << ", "
              << p1.Y() << ", "
              << p1.Z()
              << std::endl;

    std::cout << "P2: "
              << p2.X() << ", "
              << p2.Y() << ", "
              << p2.Z()
              << std::endl;

    std::cout << "P3: "
              << p3.X() << ", "
              << p3.Y() << ", "
              << p3.Z()
              << std::endl;

    std::cout << "P4: "
              << p4.X() << ", "
              << p4.Y() << ", "
              << p4.Z()
              << std::endl;


    // ============================================================
    // 2. Create a closed polygon
    // ============================================================

    BRepBuilderAPI_MakePolygon polygon;

    polygon.Add(p1);
    polygon.Add(p2);
    polygon.Add(p3);
    polygon.Add(p4);
    polygon.Close();

    TopoDS_Wire wire = polygon.Wire();

    std::cout << "\nWire:"
              << std::endl;

    std::cout << "Wire is null: "
              << wire.IsNull()
              << std::endl;


    // ============================================================
    // 3. Create a planar face from the wire
    // ============================================================

    BRepBuilderAPI_MakeFace faceBuilder(wire);

    TopoDS_Face face = faceBuilder.Face();

    std::cout << "\nFace:"
              << std::endl;

    std::cout << "Face is null: "
              << face.IsNull()
              << std::endl;


    // ============================================================
    // 4. Inspect the underlying surface
    // ============================================================

    Handle(Geom_Surface) surface =
        BRep_Tool::Surface(face);

    std::cout << "\nUnderlying surface:"
              << std::endl;

    std::cout << "Surface exists: "
              << !surface.IsNull()
              << std::endl;


    // ============================================================
    // 5. Traverse the face's edges
    // ============================================================

    std::cout << "\nFace topology:"
              << std::endl;

    int edgeCount = 0;

    for (TopExp_Explorer explorer(
             face,
             TopAbs_EDGE);
         explorer.More();
         explorer.Next())
    {
        TopoDS_Edge edge =
            TopoDS::Edge(explorer.Current());

        edgeCount++;

        std::cout << "\nEdge "
                  << edgeCount
                  << ":"
                  << std::endl;

        std::cout << "  Null: "
                  << edge.IsNull()
                  << std::endl;


        // --------------------------------------------------------
        // Extract vertices belonging to this edge
        // --------------------------------------------------------

        int vertexCount = 0;

        for (TopExp_Explorer vertexExplorer(
                 edge,
                 TopAbs_VERTEX);
             vertexExplorer.More();
             vertexExplorer.Next())
        {
            TopoDS_Vertex vertex =
                TopoDS::Vertex(
                    vertexExplorer.Current()
                );

            vertexCount++;

            gp_Pnt point =
                BRep_Tool::Pnt(vertex);

            std::cout << "  Vertex "
                      << vertexCount
                      << ": "
                      << point.X() << ", "
                      << point.Y() << ", "
                      << point.Z()
                      << std::endl;
        }

        std::cout << "  Vertex count: "
                  << vertexCount
                  << std::endl;
    }


    // ============================================================
    // 6. Count vertices of the entire face
    // ============================================================

    int faceVertexCount = 0;

    for (TopExp_Explorer explorer(
             face,
             TopAbs_VERTEX);
         explorer.More();
         explorer.Next())
    {
        faceVertexCount++;
    }

    std::cout << "\nFace vertex count: "
              << faceVertexCount
              << std::endl;


    // ============================================================
    // 7. Count wires
    // ============================================================

    int wireCount = 0;

    for (TopExp_Explorer explorer(
             face,
             TopAbs_WIRE);
         explorer.More();
         explorer.Next())
    {
        wireCount++;
    }

    std::cout << "Face wire count: "
              << wireCount
              << std::endl;


    // ============================================================
    // 8. Basic verification
    // ============================================================

    double width =
        p1.Distance(p2);

    double height =
        p2.Distance(p3);

    double expectedArea =
        width * height;

    std::cout << "\nGeometry verification:"
              << std::endl;

    std::cout << "Width: "
              << width
              << std::endl;

    std::cout << "Height: "
              << height
              << std::endl;

    std::cout << "Expected area: "
              << expectedArea
              << std::endl;


    std::cout << "\nTopology verification:"
              << std::endl;

    std::cout << "Expected edges: 4"
              << std::endl;

    std::cout << "Actual edges: "
              << edgeCount
              << std::endl;

    std::cout << "Expected wires: 1"
              << std::endl;

    std::cout << "Actual wires: "
              << wireCount
              << std::endl;


    return 0;
}
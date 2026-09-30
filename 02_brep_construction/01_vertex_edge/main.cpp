#include <iostream>

#include <gp_Pnt.hxx>
#include <gp_Lin.hxx>
#include <gp_Dir.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>

#include <BRep_Tool.hxx>

#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>

int main()
{
    // ============================================================
    // 1. Create two geometric points
    // ============================================================

    gp_Pnt p1(0.0, 0.0, 0.0);
    gp_Pnt p2(10.0, 0.0, 0.0);

    std::cout << "Geometric points:"
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


    // ============================================================
    // 2. Create topological vertices
    // ============================================================

    TopoDS_Vertex vertex1 =
        BRepBuilderAPI_MakeVertex(p1);

    TopoDS_Vertex vertex2 =
        BRepBuilderAPI_MakeVertex(p2);

    std::cout << "\nVertices:"
              << std::endl;

    std::cout << "Vertex 1 created: "
              << !vertex1.IsNull()
              << std::endl;

    std::cout << "Vertex 2 created: "
              << !vertex2.IsNull()
              << std::endl;


    // ============================================================
    // 3. Recover geometry from the topological vertices
    // ============================================================

    gp_Pnt recoveredP1 =
        BRep_Tool::Pnt(vertex1);

    gp_Pnt recoveredP2 =
        BRep_Tool::Pnt(vertex2);

    std::cout << "\nRecovered vertex geometry:"
              << std::endl;

    std::cout << "Vertex 1: "
              << recoveredP1.X() << ", "
              << recoveredP1.Y() << ", "
              << recoveredP1.Z()
              << std::endl;

    std::cout << "Vertex 2: "
              << recoveredP2.X() << ", "
              << recoveredP2.Y() << ", "
              << recoveredP2.Z()
              << std::endl;


    // ============================================================
    // 4. Create an edge between the two points
    // ============================================================

    TopoDS_Edge edge =
        BRepBuilderAPI_MakeEdge(p1, p2);

    std::cout << "\nEdge:"
              << std::endl;

    std::cout << "Edge created: "
              << !edge.IsNull()
              << std::endl;


    // ============================================================
    // 5. Extract the underlying curve
    // ============================================================

    Standard_Real firstParameter;
    Standard_Real lastParameter;

    Handle(Geom_Curve) curve =
        BRep_Tool::Curve(
            edge,
            firstParameter,
            lastParameter
        );

    std::cout << "\nUnderlying geometry:"
              << std::endl;

    std::cout << "Curve exists: "
              << !curve.IsNull()
              << std::endl;

    std::cout << "First parameter: "
              << firstParameter
              << std::endl;

    std::cout << "Last parameter: "
              << lastParameter
              << std::endl;


    // ============================================================
    // 6. Verify edge length
    // ============================================================

    double edgeLength =
        p1.Distance(p2);

    std::cout << "\nEdge geometry:"
              << std::endl;

    std::cout << "Expected length: "
              << edgeLength
              << std::endl;


    // ============================================================
    // 7. Basic topology verification
    // ============================================================

    std::cout << "\nTopology verification:"
              << std::endl;

    std::cout << "Edge is null: "
              << edge.IsNull()
              << std::endl;

    std::cout << "Vertex 1 is null: "
              << vertex1.IsNull()
              << std::endl;

    std::cout << "Vertex 2 is null: "
              << vertex2.IsNull()
              << std::endl;


    return 0;
}

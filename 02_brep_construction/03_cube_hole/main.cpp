#include <iostream>

#include <gp_Pnt.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>

// Primitive construction
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>

// Boolean operations
#include <BRepAlgoAPI_Cut.hxx>

// B-rep geometry/topology
#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>

// Geometry
#include <Geom_Surface.hxx>

// STEP
#include <STEPControl_Writer.hxx>
#include <STEPControl_StepModelType.hxx>
#include <IFSelect_ReturnStatus.hxx>


int main()
{
    // ============================================================
    // 1. Create cube
    // ============================================================

    const double width  = 20.0;
    const double depth  = 20.0;
    const double height = 20.0;

    TopoDS_Shape box =
        BRepPrimAPI_MakeBox(
            width,
            depth,
            height
        ).Shape();

    std::cout << "Cube" << std::endl;
    std::cout << "-----" << std::endl;

    std::cout << "Width:  " << width << std::endl;
    std::cout << "Depth:  " << depth << std::endl;
    std::cout << "Height: " << height << std::endl;

    std::cout << "Box is null: "
              << box.IsNull()
              << std::endl;


    // ============================================================
    // 2. Create cylindrical cutting tool
    //
    // Cube:
    //     x = 0 ... 20
    //     y = 0 ... 20
    //     z = 0 ... 20
    //
    // Hole:
    //     center = (10, 10)
    //     radius = 5
    //
    // The cylinder extends slightly beyond the cube:
    //     z = -1 ... 21
    //
    // This makes a true through-hole and avoids coincident
    // cylinder/cube faces at z = 0 and z = 20.
    // ============================================================

    const double radius = 5.0;

    const double toolStartZ = -1.0;
    const double toolHeight  = 22.0;

    gp_Pnt cylinderOrigin(
        width / 2.0,
        depth / 2.0,
        toolStartZ
    );

    gp_Dir cylinderDirection(
        0.0,
        0.0,
        1.0
    );

    gp_Ax2 cylinderAxis(
        cylinderOrigin,
        cylinderDirection
    );

    TopoDS_Shape cylinder =
        BRepPrimAPI_MakeCylinder(
            cylinderAxis,
            radius,
            toolHeight
        ).Shape();

    std::cout << "\nCylinder" << std::endl;
    std::cout << "--------" << std::endl;

    std::cout << "Radius: "
              << radius
              << std::endl;

    std::cout << "Start Z: "
              << toolStartZ
              << std::endl;

    std::cout << "Height: "
              << toolHeight
              << std::endl;

    std::cout << "Cylinder is null: "
              << cylinder.IsNull()
              << std::endl;


    // ============================================================
    // 3. Boolean cut
    //
    // Result:
    //
    //     cube - cylinder
    //
    // = cube with cylindrical through-hole
    // ============================================================

    std::cout << "\nBoolean cut" << std::endl;
    std::cout << "-----------" << std::endl;

    BRepAlgoAPI_Cut cut(
        box,
        cylinder
    );

    if (!cut.IsDone())
    {
        std::cerr << "ERROR: Boolean cut failed."
                  << std::endl;

        return 1;
    }

    TopoDS_Shape result =
        cut.Shape();

    if (result.IsNull())
    {
        std::cerr << "ERROR: Boolean result is null."
                  << std::endl;

        return 1;
    }

    std::cout << "Boolean cut completed."
              << std::endl;

    std::cout << "Result is null: "
              << result.IsNull()
              << std::endl;


    // ============================================================
    // 4. Count solids
    // ============================================================

    int solidCount = 0;

    for (TopExp_Explorer explorer(
             result,
             TopAbs_SOLID);
         explorer.More();
         explorer.Next())
    {
        ++solidCount;
    }


    // ============================================================
    // 5. Count shells
    // ============================================================

    int shellCount = 0;

    for (TopExp_Explorer explorer(
             result,
             TopAbs_SHELL);
         explorer.More();
         explorer.Next())
    {
        ++shellCount;
    }


    // ============================================================
    // 6. Count faces
    // ============================================================

    int faceCount = 0;

    for (TopExp_Explorer explorer(
             result,
             TopAbs_FACE);
         explorer.More();
         explorer.Next())
    {
        ++faceCount;
    }


    // ============================================================
    // 7. Count edges
    // ============================================================

    int edgeCount = 0;

    for (TopExp_Explorer explorer(
             result,
             TopAbs_EDGE);
         explorer.More();
         explorer.Next())
    {
        ++edgeCount;
    }


    // ============================================================
    // 8. Count vertices
    // ============================================================

    int vertexCount = 0;

    for (TopExp_Explorer explorer(
             result,
             TopAbs_VERTEX);
         explorer.More();
         explorer.Next())
    {
        ++vertexCount;
    }


    // ============================================================
    // 9. Print topology
    // ============================================================

    std::cout << "\nB-rep topology" << std::endl;
    std::cout << "--------------" << std::endl;

    std::cout << "Solids:   "
              << solidCount
              << std::endl;

    std::cout << "Shells:   "
              << shellCount
              << std::endl;

    std::cout << "Faces:    "
              << faceCount
              << std::endl;

    std::cout << "Edges:    "
              << edgeCount
              << std::endl;

    std::cout << "Vertices: "
              << vertexCount
              << std::endl;


    // ============================================================
    // 10. Traverse faces
    // ============================================================

    std::cout << "\nFaces" << std::endl;
    std::cout << "-----" << std::endl;

    int faceIndex = 0;

    for (TopExp_Explorer explorer(
             result,
             TopAbs_FACE);
         explorer.More();
         explorer.Next())
    {
        TopoDS_Face face =
            TopoDS::Face(
                explorer.Current()
            );

        ++faceIndex;

        Handle(Geom_Surface) surface =
            BRep_Tool::Surface(face);

        int faceEdgeCount = 0;

        for (TopExp_Explorer edgeExplorer(
                 face,
                 TopAbs_EDGE);
             edgeExplorer.More();
             edgeExplorer.Next())
        {
            ++faceEdgeCount;
        }

        std::cout << "Face "
                  << faceIndex
                  << std::endl;

        std::cout << "  Surface exists: "
                  << !surface.IsNull()
                  << std::endl;

        std::cout << "  Edges: "
                  << faceEdgeCount
                  << std::endl;
    }


    // ============================================================
    // 11. Traverse vertices
    // ============================================================

    std::cout << "\nVertices" << std::endl;
    std::cout << "--------" << std::endl;

    int vertexIndex = 0;

    for (TopExp_Explorer explorer(
             result,
             TopAbs_VERTEX);
         explorer.More();
         explorer.Next())
    {
        TopoDS_Vertex vertex =
            TopoDS::Vertex(
                explorer.Current()
            );

        gp_Pnt point =
            BRep_Tool::Pnt(vertex);

        ++vertexIndex;

        std::cout << "Vertex "
                  << vertexIndex
                  << ": "
                  << point.X() << ", "
                  << point.Y() << ", "
                  << point.Z()
                  << std::endl;
    }


    // ============================================================
    // 12. Export to STEP
    // ============================================================

    const char* outputFile =
        "cube_hole.step";

    STEPControl_Writer writer;

    IFSelect_ReturnStatus transferStatus =
        writer.Transfer(
            result,
            STEPControl_AsIs
        );

    std::cout << "\nSTEP export" << std::endl;
    std::cout << "-----------" << std::endl;

    std::cout << "Transfer status: "
              << transferStatus
              << std::endl;

    if (transferStatus != IFSelect_RetDone)
    {
        std::cerr << "ERROR: STEP transfer failed."
                  << std::endl;

        return 1;
    }

    IFSelect_ReturnStatus writeStatus =
        writer.Write(outputFile);

    std::cout << "Write status: "
              << writeStatus
              << std::endl;

    if (writeStatus != IFSelect_RetDone)
    {
        std::cerr << "ERROR: STEP file could not be written."
                  << std::endl;

        return 1;
    }

    std::cout << "STEP file written: "
              << outputFile
              << std::endl;


    return 0;
}

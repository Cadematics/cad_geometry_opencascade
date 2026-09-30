#include <iostream>

#include <TopoDS.hxx>
#include <gp_Pnt.hxx>


#include <STEPControl_Reader.hxx>
#include <IFSelect_ReturnStatus.hxx>

#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>

#include <TopExp_Explorer.hxx>

#include <BRep_Tool.hxx>
#include <Geom_Surface.hxx>


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
    // 2. Create STEP reader
    // ============================================================

    STEPControl_Reader reader;


    // ============================================================
    // 3. Read STEP file
    // ============================================================

    IFSelect_ReturnStatus status =
        reader.ReadFile(filename);

    if (status != IFSelect_RetDone)
    {
        std::cerr << "ERROR: Could not read STEP file."
                  << std::endl;

        return 1;
    }

    std::cout << "STEP file loaded:"
              << std::endl;

    std::cout << filename
              << std::endl;


    // ============================================================
    // 4. Transfer STEP data into OCCT shapes
    // ============================================================

    Standard_Integer numberOfRoots =
        reader.NbRootsForTransfer();

    std::cout << "\nSTEP roots:"
              << std::endl;

    std::cout << "Roots: "
              << numberOfRoots
              << std::endl;

    if (!reader.TransferRoots())
    {
        std::cerr << "ERROR: Could not transfer STEP data."
                  << std::endl;

        return 1;
    }


    // ============================================================
    // 5. Get resulting shape
    // ============================================================

    TopoDS_Shape shape =
        reader.OneShape();

    if (shape.IsNull())
    {
        std::cerr << "ERROR: Resulting shape is null."
                  << std::endl;

        return 1;
    }

    std::cout << "\nShape:"
              << std::endl;

    std::cout << "Shape is null: "
              << shape.IsNull()
              << std::endl;


    // ============================================================
    // 6. Count solids
    // ============================================================

    int solidCount = 0;

    for (TopExp_Explorer explorer(
             shape,
             TopAbs_SOLID);
         explorer.More();
         explorer.Next())
    {
        solidCount++;
    }


    // ============================================================
    // 7. Count shells
    // ============================================================

    int shellCount = 0;

    for (TopExp_Explorer explorer(
             shape,
             TopAbs_SHELL);
         explorer.More();
         explorer.Next())
    {
        shellCount++;
    }


    // ============================================================
    // 8. Count faces
    // ============================================================

    int faceCount = 0;

    for (TopExp_Explorer explorer(
             shape,
             TopAbs_FACE);
         explorer.More();
         explorer.Next())
    {
        faceCount++;
    }


    // ============================================================
    // 9. Count edges
    // ============================================================

    int edgeCount = 0;

    for (TopExp_Explorer explorer(
             shape,
             TopAbs_EDGE);
         explorer.More();
         explorer.Next())
    {
        edgeCount++;
    }


    // ============================================================
    // 10. Count vertices
    // ============================================================

    int vertexCount = 0;

    for (TopExp_Explorer explorer(
             shape,
             TopAbs_VERTEX);
         explorer.More();
         explorer.Next())
    {
        vertexCount++;
    }


    // ============================================================
    // 11. Print topology summary
    // ============================================================

    std::cout << "\nB-rep topology:"
              << std::endl;

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
    // 12. Traverse faces
    // ============================================================

    std::cout << "\nFaces:"
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

        int edgeCountForFace = 0;

        for (TopExp_Explorer edgeExplorer(
                 face,
                 TopAbs_EDGE);
             edgeExplorer.More();
             edgeExplorer.Next())
        {
            edgeCountForFace++;
        }

        std::cout << "Face "
                  << faceIndex
                  << ": "
                  << "surface="
                  << !surface.IsNull()
                  << ", edges="
                  << edgeCountForFace
                  << std::endl;
    }


    // ============================================================
    // 13. Traverse vertices
    // ============================================================

    std::cout << "\nVertices:"
              << std::endl;

    int vertexIndex = 0;

    for (TopExp_Explorer explorer(
             shape,
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

        vertexIndex++;

        std::cout << "Vertex "
                  << vertexIndex
                  << ": "
                  << point.X()
                  << ", "
                  << point.Y()
                  << ", "
                  << point.Z()
                  << std::endl;
    }


    return 0;
}
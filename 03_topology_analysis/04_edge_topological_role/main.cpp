#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include <STEPControl_Reader.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>

#include <TopAbs.hxx>
#include <TopExp.hxx>

#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>

#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>

// ------------------------------------------------------------
// Load STEP file
// ------------------------------------------------------------

TopoDS_Shape loadSTEP(const std::string& filename)
{
    STEPControl_Reader reader;

    IFSelect_ReturnStatus status =
        reader.ReadFile(filename.c_str());

    if (status != IFSelect_RetDone)
    {
        throw std::runtime_error(
            "Failed to read STEP file: " + filename
        );
    }

    Standard_Integer transferred =
        reader.TransferRoots();

    if (transferred == 0)
    {
        throw std::runtime_error(
            "No STEP roots were transferred: " + filename
        );
    }

    return reader.OneShape();
}


// ------------------------------------------------------------
// Determine whether a face is already in a list
// ------------------------------------------------------------

bool containsSameFace(
    const TopoDS_Face& face,
    const std::vector<TopoDS_Face>& faces)
{
    for (const TopoDS_Face& existing : faces)
    {
        if (face.IsSame(existing))
        {
            return true;
        }
    }

    return false;
}


// ------------------------------------------------------------
// Classify the topological role of an edge
// ------------------------------------------------------------

std::string classifyEdge(
    const TopoDS_Edge& edge,
    const TopTools_IndexedDataMapOfShapeListOfShape& edgeFaceMap)
{
    // --------------------------------------------------------
    // Degenerate edge
    // --------------------------------------------------------

    if (BRep_Tool::Degenerated(edge))
    {
        return "Degenerate";
    }

    // --------------------------------------------------------
    // Find incident faces
    // --------------------------------------------------------

    int edgeId = edgeFaceMap.FindIndex(edge);

    if (edgeId == 0)
    {
        return "Unknown";
    }

    const TopTools_ListOfShape& faces =
        edgeFaceMap.FindFromIndex(edgeId);

    // --------------------------------------------------------
    // No incident faces
    // --------------------------------------------------------

    if (faces.IsEmpty())
    {
        return "Unknown";
    }

    // --------------------------------------------------------
    // One face occurrence
    //
    // This is a boundary edge.
    // --------------------------------------------------------

    if (faces.Extent() == 1)
    {
        return "Boundary";
    }

    // --------------------------------------------------------
    // Determine the number of DISTINCT faces.
    //
    // A seam edge can occur twice in the same face:
    //
    //     F7 F7
    //
    // A normal shared edge has different faces:
    //
    //     F1 F2
    // --------------------------------------------------------

    std::vector<TopoDS_Face> uniqueFaces;

    for (TopTools_ListIteratorOfListOfShape it(faces);
         it.More();
         it.Next())
    {
        TopoDS_Face face =
            TopoDS::Face(it.Value());

        if (!containsSameFace(face, uniqueFaces))
        {
            uniqueFaces.push_back(face);
        }
    }

    // --------------------------------------------------------
    // Same face appears multiple times -> seam
    // --------------------------------------------------------

    if (uniqueFaces.size() == 1)
    {
        return "Seam";
    }

    // --------------------------------------------------------
    // Multiple distinct faces -> shared
    // --------------------------------------------------------

    return "Shared";
}


// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr
            << "Usage: " << argv[0]
            << " <file.step>\n";

        return 1;
    }

    const std::string filename = argv[1];

    try
    {
        // ----------------------------------------------------
        // Load STEP model
        // ----------------------------------------------------

        TopoDS_Shape shape =
            loadSTEP(filename);

        // ----------------------------------------------------
        // Create stable topology maps
        // ----------------------------------------------------

        TopTools_IndexedMapOfShape edgeMap;
        TopTools_IndexedMapOfShape vertexMap;
        TopTools_IndexedMapOfShape faceMap;

        TopExp::MapShapes(
            shape,
            TopAbs_EDGE,
            edgeMap
        );

        TopExp::MapShapes(
            shape,
            TopAbs_VERTEX,
            vertexMap
        );

        TopExp::MapShapes(
            shape,
            TopAbs_FACE,
            faceMap
        );

        // ----------------------------------------------------
        // Build:
        //
        // Edge -> Faces
        // ----------------------------------------------------

        TopTools_IndexedDataMapOfShapeListOfShape edgeFaceMap;

        TopExp::MapShapesAndAncestors(
            shape,
            TopAbs_EDGE,
            TopAbs_FACE,
            edgeFaceMap
        );

        // ----------------------------------------------------
        // Header
        // ----------------------------------------------------

        std::cout
            << "EDGE TOPOLOGICAL ROLE CLASSIFICATION\n"
            << "=====================================\n\n";

        std::cout
            << "File: " << filename << "\n"
            << "Edges: " << edgeMap.Extent() << "\n\n";

        // ----------------------------------------------------
        // Statistics
        // ----------------------------------------------------

        std::map<std::string, int> counts;

        // ----------------------------------------------------
        // Analyze every edge
        // ----------------------------------------------------

        for (int i = 1;
             i <= edgeMap.Extent();
             ++i)
        {
            TopoDS_Edge edge =
                TopoDS::Edge(edgeMap(i));

            std::string role =
                classifyEdge(
                    edge,
                    edgeFaceMap
                );

            counts[role]++;

            std::cout
                << "Edge " << i << "\n";

            std::cout
                << "  Topological role: "
                << role << "\n";

            // ------------------------------------------------
            // Vertices
            // ------------------------------------------------

            std::cout
                << "  Vertices:";

            for (TopExp_Explorer exp(
                     edge,
                     TopAbs_VERTEX);
                 exp.More();
                 exp.Next())
            {
                int vertexId =
                    vertexMap.FindIndex(
                        exp.Current()
                    );

                std::cout
                    << " V"
                    << vertexId;
            }

            std::cout << "\n";

            // ------------------------------------------------
            // Incident faces
            // ------------------------------------------------

            std::cout
                << "  Faces:";

            int edgeId =
                edgeFaceMap.FindIndex(edge);

            if (edgeId > 0)
            {
                const TopTools_ListOfShape& faces =
                    edgeFaceMap.FindFromIndex(edgeId);

                for (TopTools_ListIteratorOfListOfShape it(faces);
                     it.More();
                     it.Next())
                {
                    int faceId =
                        faceMap.FindIndex(
                            it.Value()
                        );

                    std::cout
                        << " F"
                        << faceId;
                }
            }

            std::cout << "\n\n";
        }

        // ----------------------------------------------------
        // Summary
        // ----------------------------------------------------

        std::cout
            << "SUMMARY\n"
            << "=======\n";

        for (const auto& [role, count] : counts)
        {
            std::cout
                << role
                << ": "
                << count
                << "\n";
        }

        std::cout << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr
            << "Error: "
            << e.what()
            << "\n";

        return 1;
    }

    return 0;
}
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>

#include <STEPControl_Reader.hxx>

#include <TopoDS_Shape.hxx>

#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>

#include <TopAbs_ShapeEnum.hxx>

#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>


// ============================================================
// STEP loading
// ============================================================

TopoDS_Shape loadStep(const std::string& filename)
{
    STEPControl_Reader reader;

    IFSelect_ReturnStatus status =
        reader.ReadFile(filename.c_str());

    if (status != IFSelect_RetDone)
    {
        std::cerr << "Failed to read STEP file.\n";
        return TopoDS_Shape();
    }

    if (!reader.TransferRoots())
    {
        std::cerr << "Failed to transfer STEP roots.\n";
        return TopoDS_Shape();
    }

    return reader.OneShape();
}


// ============================================================
// Main
// ============================================================

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr
            << "Usage: ./incidence_test <file.step>\n";

        return 1;
    }

    const std::string filename = argv[1];

    TopoDS_Shape shape = loadStep(filename);

    if (shape.IsNull())
    {
        return 1;
    }


    // ========================================================
    // 1. Build indexed topology maps
    // ========================================================

    TopTools_IndexedMapOfShape faceMap;
    TopTools_IndexedMapOfShape edgeMap;
    TopTools_IndexedMapOfShape vertexMap;

    TopExp::MapShapes(
        shape,
        TopAbs_FACE,
        faceMap
    );

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


    std::cout << "TOPOLOGY MAP\n";
    std::cout << "============\n\n";

    std::cout << "Faces:    "
              << faceMap.Extent()
              << '\n';

    std::cout << "Edges:    "
              << edgeMap.Extent()
              << '\n';

    std::cout << "Vertices: "
              << vertexMap.Extent()
              << '\n';


    // ========================================================
    // 2. Build incidence maps
    //
    // Edge -> Face
    // Vertex -> Edge
    // ========================================================

    TopTools_IndexedDataMapOfShapeListOfShape edgeFaceMap;
    TopTools_IndexedDataMapOfShapeListOfShape vertexEdgeMap;

    TopExp::MapShapesAndAncestors(
        shape,
        TopAbs_EDGE,
        TopAbs_FACE,
        edgeFaceMap
    );

    TopExp::MapShapesAndAncestors(
        shape,
        TopAbs_VERTEX,
        TopAbs_EDGE,
        vertexEdgeMap
    );


    // ========================================================
    // 3. Face -> Edge
    // ========================================================

    std::cout << "\n\nFACE -> EDGE IDs\n";
    std::cout << "================\n";

    for (int faceId = 1;
         faceId <= faceMap.Extent();
         ++faceId)
    {
        TopoDS_Shape face =
            faceMap.FindKey(faceId);

        std::cout
            << "Face "
            << faceId
            << ":";

        for (TopExp_Explorer explorer(
                 face,
                 TopAbs_EDGE);
             explorer.More();
             explorer.Next())
        {
            TopoDS_Shape edge =
                explorer.Current();

            int edgeId =
                edgeMap.FindIndex(edge);

            std::cout
                << " E"
                << edgeId;
        }

        std::cout << '\n';
    }


    // ========================================================
    // 4. Edge -> Vertex
    // ========================================================

    std::cout << "\n\nEDGE -> VERTEX IDs\n";
    std::cout << "=================\n";

    for (int edgeId = 1;
         edgeId <= edgeMap.Extent();
         ++edgeId)
    {
        TopoDS_Shape edge =
            edgeMap.FindKey(edgeId);

        std::cout
            << "Edge "
            << edgeId
            << ":";

        for (TopExp_Explorer explorer(
                 edge,
                 TopAbs_VERTEX);
             explorer.More();
             explorer.Next())
        {
            TopoDS_Shape vertex =
                explorer.Current();

            int vertexId =
                vertexMap.FindIndex(vertex);

            std::cout
                << " V"
                << vertexId;
        }

        std::cout << '\n';
    }


    // ========================================================
    // 5. Edge -> Face
    // ========================================================

    std::cout << "\n\nEDGE -> FACE IDs\n";
    std::cout << "===============\n";

    for (int edgeId = 1;
         edgeId <= edgeMap.Extent();
         ++edgeId)
    {
        TopoDS_Shape edge =
            edgeMap.FindKey(edgeId);

        std::cout
            << "Edge "
            << edgeId
            << ":";

        int mapIndex =
            edgeFaceMap.FindIndex(edge);

        if (mapIndex > 0)
        {
            const TopTools_ListOfShape& faces =
                edgeFaceMap.FindFromIndex(mapIndex);

            for (TopTools_ListIteratorOfListOfShape it(faces);
                 it.More();
                 it.Next())
            {
                int faceId =
                    faceMap.FindIndex(it.Value());

                std::cout
                    << " F"
                    << faceId;
            }
        }

        std::cout << '\n';
    }


    // ========================================================
    // 6. Vertex -> Edge
    // ========================================================

    std::cout << "\n\nVERTEX -> EDGE IDs\n";
    std::cout << "=================\n";

    for (int vertexId = 1;
         vertexId <= vertexMap.Extent();
         ++vertexId)
    {
        TopoDS_Shape vertex =
            vertexMap.FindKey(vertexId);

        std::cout
            << "Vertex "
            << vertexId
            << ":";

        int mapIndex =
            vertexEdgeMap.FindIndex(vertex);

        if (mapIndex > 0)
        {
            const TopTools_ListOfShape& edges =
                vertexEdgeMap.FindFromIndex(mapIndex);

            for (TopTools_ListIteratorOfListOfShape it(edges);
                 it.More();
                 it.Next())
            {
                int edgeId =
                    edgeMap.FindIndex(it.Value());

                std::cout
                    << " E"
                    << edgeId;
            }
        }

        std::cout << '\n';
    }


    // ========================================================
    // 7. Face -> Face adjacency
    //
    // Two faces are adjacent when they share an edge.
    //
    // Store:
    //
    //     (Face A, Face B) -> shared Edge IDs
    //
    // This also handles cases where two faces share
    // multiple edges.
    // ========================================================

    std::map<
        std::pair<int, int>,
        std::vector<int>
    > faceAdjacency;


    for (int edgeId = 1;
         edgeId <= edgeMap.Extent();
         ++edgeId)
    {
        TopoDS_Shape edge =
            edgeMap.FindKey(edgeId);

        int mapIndex =
            edgeFaceMap.FindIndex(edge);

        if (mapIndex <= 0)
        {
            continue;
        }

        const TopTools_ListOfShape& faces =
            edgeFaceMap.FindFromIndex(mapIndex);


        // ----------------------------------------------------
        // Collect unique face IDs for this edge.
        // ----------------------------------------------------

        std::vector<int> faceIds;

        for (TopTools_ListIteratorOfListOfShape it(faces);
             it.More();
             it.Next())
        {
            int faceId =
                faceMap.FindIndex(it.Value());

            if (faceId <= 0)
            {
                continue;
            }

            if (std::find(
                    faceIds.begin(),
                    faceIds.end(),
                    faceId)
                == faceIds.end())
            {
                faceIds.push_back(faceId);
            }
        }


        // ----------------------------------------------------
        // Generate distinct face pairs.
        // ----------------------------------------------------

        for (size_t i = 0;
             i < faceIds.size();
             ++i)
        {
            for (size_t j = i + 1;
                 j < faceIds.size();
                 ++j)
            {
                int faceA = faceIds[i];
                int faceB = faceIds[j];

                if (faceA > faceB)
                {
                    std::swap(faceA, faceB);
                }

                faceAdjacency[
                    {faceA, faceB}
                ].push_back(edgeId);
            }
        }
    }


    // ========================================================
    // 8. Print Face -> Face adjacency
    // ========================================================

    std::cout << "\n\nFACE ADJACENCY\n";
    std::cout << "==============\n";

    for (const auto& entry : faceAdjacency)
    {
        int faceA = entry.first.first;
        int faceB = entry.first.second;

        const std::vector<int>& sharedEdges =
            entry.second;

        std::cout
            << "Face "
            << faceA
            << " <-> Face "
            << faceB
            << " via";

        for (int edgeId : sharedEdges)
        {
            std::cout
                << " Edge "
                << edgeId;
        }

        std::cout << '\n';
    }


    // ========================================================
    // 9. Topology consistency checks
    // ========================================================

    std::cout << "\n\nTOPOLOGY CONSISTENCY CHECK\n";
    std::cout << "==========================\n";

    bool valid = true;


    // --------------------------------------------------------
    // Every face must contain at least one edge.
    // Every referenced edge must exist in edgeMap.
    // --------------------------------------------------------

    for (int faceId = 1;
         faceId <= faceMap.Extent();
         ++faceId)
    {
        TopoDS_Shape face =
            faceMap.FindKey(faceId);

        int edgeCount = 0;

        for (TopExp_Explorer explorer(
                 face,
                 TopAbs_EDGE);
             explorer.More();
             explorer.Next())
        {
            ++edgeCount;

            TopoDS_Shape edge =
                explorer.Current();

            int edgeId =
                edgeMap.FindIndex(edge);

            if (edgeId <= 0)
            {
                valid = false;

                std::cout
                    << "ERROR: Face "
                    << faceId
                    << " references an unknown edge.\n";
            }
        }

        if (edgeCount == 0)
        {
            valid = false;

            std::cout
                << "ERROR: Face "
                << faceId
                << " contains no edges.\n";
        }
    }


    // --------------------------------------------------------
    // Every edge must contain at least one vertex.
    // Every referenced vertex must exist in vertexMap.
    // --------------------------------------------------------

    for (int edgeId = 1;
         edgeId <= edgeMap.Extent();
         ++edgeId)
    {
        TopoDS_Shape edge =
            edgeMap.FindKey(edgeId);

        int vertexCount = 0;

        for (TopExp_Explorer explorer(
                 edge,
                 TopAbs_VERTEX);
             explorer.More();
             explorer.Next())
        {
            ++vertexCount;

            TopoDS_Shape vertex =
                explorer.Current();

            int vertexId =
                vertexMap.FindIndex(vertex);

            if (vertexId <= 0)
            {
                valid = false;

                std::cout
                    << "ERROR: Edge "
                    << edgeId
                    << " references an unknown vertex.\n";
            }
        }

        if (vertexCount == 0)
        {
            valid = false;

            std::cout
                << "ERROR: Edge "
                << edgeId
                << " contains no vertices.\n";
        }
    }


    // --------------------------------------------------------
    // Every edge should have at least one incident face.
    //
    // We do not require exactly two faces because:
    //
    //   - boundary edges may have one face
    //   - seam edges may reference the same face twice
    //   - non-manifold edges may have more than two faces
    // --------------------------------------------------------

    for (int edgeId = 1;
         edgeId <= edgeMap.Extent();
         ++edgeId)
    {
        TopoDS_Shape edge =
            edgeMap.FindKey(edgeId);

        int mapIndex =
            edgeFaceMap.FindIndex(edge);

        if (mapIndex <= 0)
        {
            valid = false;

            std::cout
                << "ERROR: Edge "
                << edgeId
                << " has no incident face.\n";
        }
    }


    if (valid)
    {
        std::cout
            << "Topology incidence checks passed.\n";
    }


    // ========================================================
    // 10. Stable topology ID check
    // ========================================================

    std::cout << "\n\nSTABLE ID CHECK\n";
    std::cout << "===============\n";

    bool stable = true;


    // --------------------------------------------------------
    // Verify Face -> Edge references.
    // --------------------------------------------------------

    for (int faceId = 1;
         faceId <= faceMap.Extent();
         ++faceId)
    {
        TopoDS_Shape face =
            faceMap.FindKey(faceId);

        for (TopExp_Explorer explorer(
                 face,
                 TopAbs_EDGE);
             explorer.More();
             explorer.Next())
        {
            TopoDS_Shape edge =
                explorer.Current();

            int edgeId =
                edgeMap.FindIndex(edge);

            if (edgeId <= 0)
            {
                stable = false;

                std::cout
                    << "ERROR: Edge from Face "
                    << faceId
                    << " was not found in edgeMap.\n";
            }
        }
    }


    // --------------------------------------------------------
    // Verify Edge -> Vertex references.
    // --------------------------------------------------------

    for (int edgeId = 1;
         edgeId <= edgeMap.Extent();
         ++edgeId)
    {
        TopoDS_Shape edge =
            edgeMap.FindKey(edgeId);

        for (TopExp_Explorer explorer(
                 edge,
                 TopAbs_VERTEX);
             explorer.More();
             explorer.Next())
        {
            TopoDS_Shape vertex =
                explorer.Current();

            int vertexId =
                vertexMap.FindIndex(vertex);

            if (vertexId <= 0)
            {
                stable = false;

                std::cout
                    << "ERROR: Vertex from Edge "
                    << edgeId
                    << " was not found in vertexMap.\n";
            }
        }
    }


    if (stable)
    {
        std::cout
            << "All topology references map to stable IDs.\n";
    }


    // ========================================================
    // Final result
    // ========================================================

    return (valid && stable) ? 0 : 1;
}
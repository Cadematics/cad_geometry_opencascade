#include <iostream>
#include <stdexcept>

#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>

#include <TopTools_IndexedMapOfShape.hxx>

#include "step_reader.h"
#include "topology_utils.h"
#include "face_relationships.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr
            << "Usage: "
            << argv[0]
            << " <file.step>\n";

        return 1;
    }

    const std::string filename =
        argv[1];

    try
    {
        // ------------------------------------------------------------
        // 1. Load STEP
        // ------------------------------------------------------------

        TopoDS_Shape shape =
            loadSTEP(filename);

        // ------------------------------------------------------------
        // 2. Build topology maps
        // ------------------------------------------------------------

        TopTools_IndexedMapOfShape faceMap;
        TopTools_IndexedMapOfShape edgeMap;

        buildTopologyMaps(
            shape,
            faceMap,
            edgeMap
        );

        // ------------------------------------------------------------
        // 3. Build face adjacency
        // ------------------------------------------------------------

        FaceAdjacency adjacency =
            buildFaceAdjacency(
                shape,
                faceMap,
                edgeMap
            );

        // ------------------------------------------------------------
        // 4. Print model information
        // ------------------------------------------------------------

        std::cout
            << "FACE RELATIONSHIP ANALYSIS\n"
            << "==========================\n\n";

        std::cout
            << "File: "
            << filename
            << "\n";

        std::cout
            << "Faces: "
            << faceMap.Extent()
            << "\n";

        std::cout
            << "Adjacent face pairs: "
            << adjacency.size()
            << "\n\n";

        // ------------------------------------------------------------
        // 5. Analyze each adjacent face pair
        // ------------------------------------------------------------

        for (const auto& [pair, edges] : adjacency)
        {
            int face1Id =
                pair.first;

            int face2Id =
                pair.second;

            TopoDS_Face face1 =
                TopoDS::Face(
                    faceMap(face1Id)
                );

            TopoDS_Face face2 =
                TopoDS::Face(
                    faceMap(face2Id)
                );

            std::cout
                << "Faces F"
                << face1Id
                << " <-> F"
                << face2Id
                << "\n";

            // --------------------------------------------------------
            // Shared edges
            // --------------------------------------------------------

            std::cout
                << "  Shared edges:";

            for (int edgeId : edges)
            {
                std::cout
                    << " E"
                    << edgeId;
            }

            std::cout
                << "\n";

            // --------------------------------------------------------
            // Surface types
            // --------------------------------------------------------

            FaceSurfaceType type1 =
                getSurfaceType(face1);

            FaceSurfaceType type2 =
                getSurfaceType(face2);

            std::cout
                << "  Face F"
                << face1Id
                << " type: "
                << surfaceTypeToString(type1)
                << "\n";

            std::cout
                << "  Face F"
                << face2Id
                << " type: "
                << surfaceTypeToString(type2)
                << "\n";

            // --------------------------------------------------------
            // Geometric relationship
            // --------------------------------------------------------

            FaceRelationship relationship =
                classifyFaceRelationship(
                    face1,
                    face2
                );

            std::cout
                << "  Relationship: "
                << relationshipToString(
                    relationship
                )
                << "\n";

            // --------------------------------------------------------
            // Normal angle
            //
            // Only meaningful when both faces are planar.
            // --------------------------------------------------------

            gp_Dir normal1;
            gp_Dir normal2;

            bool hasNormal1 =
                getPlaneNormal(
                    face1,
                    normal1
                );

            bool hasNormal2 =
                getPlaneNormal(
                    face2,
                    normal2
                );

            if (hasNormal1 && hasNormal2)
            {
                double angle =
                    angleBetweenNormals(
                        normal1,
                        normal2
                    );

                std::cout
                    << "  Normal angle: "
                    << angle
                    << " degrees\n";
            }

            std::cout
                << "\n";
        }
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

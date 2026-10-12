#include "topology_utils.h"

#include <algorithm>
#include <vector>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>

#include <TopAbs.hxx>
#include <TopExp.hxx>

#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>


void buildTopologyMaps(
    const TopoDS_Shape& shape,
    TopTools_IndexedMapOfShape& faceMap,
    TopTools_IndexedMapOfShape& edgeMap)
{
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
}


FaceAdjacency buildFaceAdjacency(
    const TopoDS_Shape& shape,
    const TopTools_IndexedMapOfShape& faceMap,
    const TopTools_IndexedMapOfShape& edgeMap)
{
    FaceAdjacency faceAdjacency;

    TopTools_IndexedDataMapOfShapeListOfShape
        edgeFaceMap;

    TopExp::MapShapesAndAncestors(
        shape,
        TopAbs_EDGE,
        TopAbs_FACE,
        edgeFaceMap
    );


    for (int edgeId = 1;
         edgeId <= edgeMap.Extent();
         ++edgeId)
    {
        TopoDS_Edge edge =
            TopoDS::Edge(edgeMap(edgeId));

        int edgeIndex =
            edgeFaceMap.FindIndex(edge);

        if (edgeIndex == 0)
        {
            continue;
        }

        const TopTools_ListOfShape& faces =
            edgeFaceMap.FindFromIndex(edgeIndex);

        std::vector<int> faceIds;

        for (TopTools_ListIteratorOfListOfShape it(faces);
             it.More();
             it.Next())
        {
            int faceId =
                faceMap.FindIndex(it.Value());

            if (faceId > 0)
            {
                faceIds.push_back(faceId);
            }
        }


        // Create all pairs of distinct faces
        // that share this edge.

        for (std::size_t i = 0;
             i < faceIds.size();
             ++i)
        {
            for (std::size_t j = i + 1;
                 j < faceIds.size();
                 ++j)
            {
                int face1 = faceIds[i];
                int face2 = faceIds[j];

                if (face1 == face2)
                {
                    continue;
                }

                if (face1 > face2)
                {
                    std::swap(face1, face2);
                }

                faceAdjacency[
                    {face1, face2}
                ].insert(edgeId);
            }
        }
    }

    return faceAdjacency;
}
#ifndef TOPOLOGY_UTILS_H
#define TOPOLOGY_UTILS_H

#include <map>
#include <set>
#include <utility>

#include <TopoDS_Shape.hxx>

#include <TopTools_IndexedMapOfShape.hxx>


using FaceAdjacency = std::map< std::pair<int, int>, std::set<int>>;


void buildTopologyMaps(
    const TopoDS_Shape& shape,
    TopTools_IndexedMapOfShape& faceMap,
    TopTools_IndexedMapOfShape& edgeMap
);


FaceAdjacency buildFaceAdjacency(
    const TopoDS_Shape& shape,
    const TopTools_IndexedMapOfShape& faceMap,
    const TopTools_IndexedMapOfShape& edgeMap
);

#endif
#include <iostream>
#include <string>

#include <STEPControl_Reader.hxx>

#include <BRepAdaptor_Curve.hxx>

#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>

#include <TopAbs_ShapeEnum.hxx>

#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>

#include <BRep_Tool.hxx>
#include <BRepAdaptor_Curve.hxx>

#include <Geom_Curve.hxx>
#include <Geom_Line.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_Parabola.hxx>
#include <Geom_Hyperbola.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_BSplineCurve.hxx>

#include <GCPnts_AbscissaPoint.hxx>


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
// Curve classification
// ============================================================

std::string classifyCurve(
    const Handle(Geom_Curve)& curve)
{
    if (curve.IsNull())
    {
        return "None";
    }

    if (!Handle(Geom_Line)::DownCast(curve).IsNull())
    {
        return "Line";
    }

    if (!Handle(Geom_Circle)::DownCast(curve).IsNull())
    {
        return "Circle";
    }

    if (!Handle(Geom_Ellipse)::DownCast(curve).IsNull())
    {
        return "Ellipse";
    }

    if (!Handle(Geom_Parabola)::DownCast(curve).IsNull())
    {
        return "Parabola";
    }

    if (!Handle(Geom_Hyperbola)::DownCast(curve).IsNull())
    {
        return "Hyperbola";
    }

    if (!Handle(Geom_BezierCurve)::DownCast(curve).IsNull())
    {
        return "Bezier";
    }

    if (!Handle(Geom_BSplineCurve)::DownCast(curve).IsNull())
    {
        return "BSpline";
    }

    return "Other";
}


// ============================================================
// Curve length
// ============================================================

double curveLength(
    const TopoDS_Edge& edge)
{
    BRepAdaptor_Curve adaptor(edge);

    return GCPnts_AbscissaPoint::Length(
        adaptor,
        adaptor.FirstParameter(),
        adaptor.LastParameter());
}


// ============================================================
// Main
// ============================================================

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr
            << "Usage: ./edge_classification <file.step>\n";

        return 1;
    }

    const std::string filename = argv[1];

    TopoDS_Shape shape =
        loadStep(filename);

    if (shape.IsNull())
    {
        return 1;
    }


    // ========================================================
    // Build topology maps
    // ========================================================

    TopTools_IndexedMapOfShape edgeMap;
    TopTools_IndexedMapOfShape vertexMap;
    TopTools_IndexedMapOfShape faceMap;

    TopExp::MapShapes(
        shape,
        TopAbs_EDGE,
        edgeMap);

    TopExp::MapShapes(
        shape,
        TopAbs_VERTEX,
        vertexMap);

    TopExp::MapShapes(
        shape,
        TopAbs_FACE,
        faceMap);


    // ========================================================
    // Edge -> Face incidence
    // ========================================================

    TopTools_IndexedDataMapOfShapeListOfShape edgeFaceMap;

    TopExp::MapShapesAndAncestors(
        shape,
        TopAbs_EDGE,
        TopAbs_FACE,
        edgeFaceMap);


    // ========================================================
    // Header
    // ========================================================

    std::cout
        << "EDGE CLASSIFICATION\n";

    std::cout
        << "===================\n\n";

    std::cout
        << "Edges: "
        << edgeMap.Extent()
        << "\n";


    // ========================================================
    // Analyze every edge
    // ========================================================

    for (int edgeId = 1;
         edgeId <= edgeMap.Extent();
         ++edgeId)
    {
        TopoDS_Edge edge =
            TopoDS::Edge(
                edgeMap.FindKey(edgeId));


        // ----------------------------------------------------
        // Curve
        // ----------------------------------------------------

        Standard_Real first;
        Standard_Real last;

        Handle(Geom_Curve) curve =
            BRep_Tool::Curve(
                edge,
                first,
                last);

        std::string curveType =
            classifyCurve(curve);


        // ----------------------------------------------------
        // Length
        // ----------------------------------------------------

        double length =
            curveLength(edge);


        // ----------------------------------------------------
        // Print edge
        // ----------------------------------------------------

        std::cout
            << "\nEdge "
            << edgeId
            << '\n';

        std::cout
            << "  Curve type: "
            << curveType
            << '\n';

        std::cout
            << "  Length: "
            << length
            << '\n';


        // ----------------------------------------------------
        // Vertices
        // ----------------------------------------------------

        std::cout
            << "  Vertices:";

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

        std::cout
            << '\n';


        // ----------------------------------------------------
        // Faces
        // ----------------------------------------------------

        std::cout
            << "  Faces:";

        int mapIndex =
            edgeFaceMap.FindIndex(edge);

        if (mapIndex > 0)
        {
            const TopTools_ListOfShape& faces =
                edgeFaceMap.FindFromIndex(
                    mapIndex);

            bool printed = false;

            for (TopTools_ListIteratorOfListOfShape it(faces);
                 it.More();
                 it.Next())
            {
                int faceId =
                    faceMap.FindIndex(
                        it.Value());

                if (faceId > 0)
                {
                    std::cout
                        << " F"
                        << faceId;

                    printed = true;
                }
            }

            if (!printed)
            {
                std::cout
                    << " None";
            }
        }
        else
        {
            std::cout
                << " None";
        }

        std::cout
            << '\n';
    }


    return 0;
}
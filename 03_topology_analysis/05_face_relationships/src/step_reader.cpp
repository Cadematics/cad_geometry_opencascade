#include "step_reader.h"

#include <stdexcept>

#include <STEPControl_Reader.hxx>


TopoDS_Shape loadSTEP(
    const std::string& filename)
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
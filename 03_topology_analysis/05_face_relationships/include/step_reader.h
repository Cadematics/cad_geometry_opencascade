#ifndef STEP_READER_H
#define STEP_READER_H

#include <string>

#include <TopoDS_Shape.hxx>


TopoDS_Shape loadSTEP(
    const std::string& filename
);










#endif
#ifndef PythonWrapper_h
#define PythonWrapper_h

#include <boost/python.hpp>

namespace python = boost::python;

class PythonWrapper {
private:
    static PythonWrapper* instance;
    void init();

public:
    static PythonWrapper* getInstance();

    python::object createObject(std::string moduleName, std::string className);
};

#endif

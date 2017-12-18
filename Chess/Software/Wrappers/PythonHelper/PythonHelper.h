#ifndef PythonHelper_h
#define PythonHelper_h

#include <boost/python.hpp>

namespace python = boost::python;

class PythonHelper {
private:
    static PythonHelper* instance;
    void init();

public:
    static PythonHelper* getInstance();

    python::object createObject(std::string moduleName, std::string className);
};

#endif

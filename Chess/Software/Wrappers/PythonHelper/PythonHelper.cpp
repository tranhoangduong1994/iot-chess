#include "PythonHelper.h"

#include <iostream>

const char* PYTHONPATH = ".";

PythonHelper* PythonHelper::instance = NULL;

PythonHelper* PythonHelper::getInstance() {
	if (!instance) {
		instance = new PythonHelper();
		instance->init();
	}
	return instance;
}

void PythonHelper::init() {
	std::cout << "[PythonWrapper] init()" << std::endl;
	setenv("PYTHONPATH", PYTHONPATH, 1);
	try {
		Py_Initialize();
	} catch (const python::error_already_set) {
		PyErr_Print();
		assert(false);
	}
}

python::object PythonHelper::createObject(std::string moduleName, std::string className)  {
	std::cout << "[PythonWrapper] createObject(" << moduleName << ", " << className << ")" << std::endl;
	try {
		python::object module = python::import(moduleName.c_str());
		return module.attr(className.c_str())();
	} catch (const python::error_already_set) {
		PyErr_Print();
		assert(false);
	}
}

#include "boost/python/detail/wrap_python.hpp"
#include "PythonExecuter.h"
#include "ExposedClasses.h"
#include "ResourceManagerFunctions.h"
#include "TimerFunctions.h"
#include "../Aela_Engine.h"

#if PY_VERSION_HEX >= 0x03000000
#  define MODULE_INIT_FN(name) BOOST_PP_CAT(PyInit_, name)
#  define PYTHON_BUILTINS "builtins"
#else
#  define MODULE_INIT_FN(name) BOOST_PP_CAT(init, name)
#  define PYTHON_BUILTINS "__builtin__"
#endif

using namespace boost;

// Is this idiotic? Yes. Does it work? Yes. Did anything else work? For some reason, no.
Aela::Engine* engineInstance;

Aela::Engine* getEngine() {
	return engineInstance;
}

Aela::PythonExecuter::PythonExecuter() {
}

Aela::PythonExecuter::~PythonExecuter() {
}

int Aela::PythonExecuter::startAllowingImports(Engine& engine) {
	PyImport_AppendInittab("Aela", &MODULE_INIT_FN(Aela));
	engineInstance = &engine;
	/*try {
		scope().attr("engine") = object(ptr(&engine));
	}
	catch (python::error_already_set&) {
		AelaErrorHandling::consoleWindowWarning("Python Executer", "Failed to start. Error:");
		PyErr_Print();
	}*/
	return 0;
}

boost::python::api::object& Aela::PythonExecuter::runPython(std::string pythonCode) {
	if (!hasBeenInitialised) {
		AelaErrorHandling::consoleWindowError("Python Executer", "You cannot run Python code before calling"
			"startAllowingPythonExecution");
		return boost::python::api::object();
	}

	try {
		return python::exec(pythonCode.c_str(), builtInsNamespace);
	} catch (...) {
		AelaErrorHandling::consoleWindowWarning("Python Executer", "Failed to run python code. Error:");
		PyErr_Print();
		return object();
	}
}

boost::python::api::object& Aela::PythonExecuter::runPythonFromFile(std::string src) {
	if (!hasBeenInitialised) {
		AelaErrorHandling::consoleWindowError("Python Executer", "You cannot run Python code before calling"
			"startAllowingPythonExecution");
		return boost::python::api::object();
	}

	try {
		object o = python::exec_file(src.c_str(), builtInsNamespace);
		return o;
	} catch (...) {
		AelaErrorHandling::consoleWindowWarning("Python Executer", "Failed to run python code. Error:");
		PyErr_Print();
		return object();
	}
}

void Aela::PythonExecuter::startAllowingPythonExecution() {
	Py_Initialize();
	hasBeenInitialised = true;
	builtInsNamespace["__builtins__"] = python::import(PYTHON_BUILTINS);
}
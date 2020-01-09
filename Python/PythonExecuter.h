/*
* Class: Python Executer
* Author: Robert Ciborowski
* Date: 01/07/2019
* Description: A class used to execute Python code.
*/

#pragma once
#include <boost/python.hpp>
#include <string>

namespace Aela {
	class Engine;
	class PythonExecuter {
		public:
			PythonExecuter();
			~PythonExecuter();

			int startAllowingImports(Engine& engine);
			boost::python::api::object runPython(std::string pythonCode);
			boost::python::api::object runPythonFromFile(std::string src);
			void startAllowingPythonExecution();

		private:
			boost::python::dict builtInsNamespace;
			bool hasBeenInitialised = false;
	};
}
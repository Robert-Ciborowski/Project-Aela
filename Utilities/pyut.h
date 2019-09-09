/*
* Class: Python Utilities
* Author: Robert Ciborowski
* Date: 03/07/2019
* Description: Functions for Python.
*/

#pragma once
#include <boost/python.hpp>
#include <iostream>

using namespace boost::python;

static std::function<void()> convertToFunc(object object) {
	auto event = [object]() {
		try {
			std::function<void()> func = object;
			func();
		} catch (...) {
			std::cout << "Python error: ";
			PyErr_Print();
		}
	};

	return event;
}
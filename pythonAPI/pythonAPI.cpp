#pragma comment( lib, "python39.lib" )

#include <string>
#include <ostream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <thread>
#include <chrono>
#include <Python.h>
#include "pyhelper.hpp"
#include <vector>

std::vector<CPyObject> modules;

struct Player_t {
	float x, y, z;
	std::string name;

	std::string toString()
	{
		return "" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ";" + name;
	}
};

std::vector<Player_t>players = {
	{10, 10, 10, "name"},
	{120, 5, 10, "who"},
};

static PyObject* api_players(PyObject* self, PyObject* args)
{
	std::string data;
	for (Player_t player : players) {
		data += player.toString() + "&";
	}

	return PyUnicode_InternFromString(data.c_str());
}

static PyObject* api_foo(PyObject* self, PyObject* args)
{
	printf_s("... in C++...: foo() method\n");
	return PyLong_FromLong(51);
}

static PyObject* api_show(PyObject* self, PyObject* args)
{
	PyObject* a;
	if (PyArg_UnpackTuple(args, "", 1, 1, &a))
	{
		printf_s("C++: show(%ld)\n", PyLong_AsLong(a));
	}

	return PyLong_FromLong(0);
}

static PyObject* api_log(PyObject* self, PyObject* args) {
	PyObject* a;
	if (PyArg_UnpackTuple(args, "", 1, 1, &a))
	{
		std::cout << "[PYTHON] " << PyUnicode_AsUTF8(a) << "\n";
	}
	return PyLong_FromLong(0);
}

static struct PyMethodDef methods[] = {
	{ "foo", api_foo, METH_VARARGS, "Returns the number"},
	{ "show", api_show, METH_VARARGS, "Show a number" },
	{ "log", api_log, METH_VARARGS, "Log message" },
	{ "get_players", api_players, METH_VARARGS, "Get players on scene" },
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef modDef = {
	PyModuleDef_HEAD_INIT, "api", NULL, -1, methods,
	NULL, NULL, NULL, NULL
};

static PyObject* PyInit_api(void)
{
	return PyModule_Create(&modDef);
}

void present_hook() {
	for (CPyObject pModule : modules) {
		if (pModule) {
			CPyObject pFunc = PyObject_GetAttrString(pModule, "present_hook");
			if (pFunc && PyCallable_Check(pFunc))
			{
				CPyObject pValue = PyObject_CallObject(pFunc, NULL);

				printf_s("present_hook() = %ld\n", PyLong_AsLong(pValue));
			}
			else
			{
				printf("ERROR: function getInteger()\n");
			}
		}
	}
}

int main()
{

	PyImport_AppendInittab("api", &PyInit_api);

	CPyInstance hInstance;

	CPyObject pName = PyUnicode_FromString("main");
	CPyObject pModule = PyImport_Import(pName);

	if(pModule)
		modules.push_back(pModule);
	else {
		printf("Cant import module\n");
	}
	
	present_hook();

	return 0;
}

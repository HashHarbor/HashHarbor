#include "toolTip.h"
#include <Python.h>

std::string toolTip(const std::string& input) {
    //this is to set the python module to the currents directory, otherwise it error unable to find python module
    setenv("PYTHONPATH", ".", 1); 

    std::string inputCondense = "Make the answer short ";
    inputCondense.append(input);
    Py_Initialize();
    if (!Py_IsInitialized())
    {
        PyRun_SimpleString("print 'initialization error!' ");
        return "print 'initialization error!' ";
    }

    PyObject *name, *load_module, *func, *callfunc, *args;
    name = PyUnicode_FromString((char*)"toolTip");
    if (!name) {
        Py_Finalize();
        return "Failed to create Python string object";
    }

    load_module = PyImport_Import(name);
    Py_DECREF(name);  // Release the reference to the name object
    if (!load_module) {
        Py_Finalize();
        return "Failed to import module 'llama'";
    }

    func = PyObject_GetAttrString(load_module,(char*)"toolTip");
    if (!func || !PyCallable_Check(func)) {
        Py_XDECREF(func);  // Release the reference to the func object
        Py_DECREF(load_module);  // Release the reference to the load_module object
        Py_Finalize();
        return "Failed to get 'toolTip' function from module 'llama";
    }

    args = PyTuple_Pack(1,PyUnicode_FromString(inputCondense.c_str()));
    if (!args) {
        Py_DECREF(func);  // Release the reference to the func object
        Py_DECREF(load_module);  // Release the reference to the load_module object
        Py_Finalize();
        return "Failed to create arguments tuple";
    }

    callfunc = PyObject_CallObject(func, args);
    Py_DECREF(args);  // Release the reference to the args tuples
    Py_DECREF(func);  // Release the reference to the func object
    Py_DECREF(load_module);  // Release the reference to the load_module object
    if (!callfunc) {
        Py_Finalize();
        return "Function call failed";
    }

    std::string toolTip_output = PyUnicode_AsUTF8(callfunc);
    Py_DECREF(callfunc);  // Release the reference to the callfunc object
    Py_Finalize();

    return toolTip_output;
} 
#include <Python.h>


static PyObject* py_factorize(long long number){
  PyObject *py_result = PyList_New(0);
  for (int i = 2; i * i <= number; ++i){
    while (number % i == 0){
      PyList_Append(py_result, PyLong_FromLongLong(i));
      number /= i;
    }
  }
  if (number > 1){
    PyList_Append(py_result, PyLong_FromLongLong(number));
  }
  return py_result;
}

static PyObject* factor_out(PyObject* self, PyObject* args) {
  long long number;
  if (!PyArg_ParseTuple(args, "L", &number)) {
    PyErr_SetString(PyExc_ValueError, "not number is given");
    return NULL;
  }
  PyObject* prime_deviders = py_factorize(number);
  if (PyList_Size(prime_deviders) <= 1) {
    return PyUnicode_FromString("Prime!");
  }
  return prime_deviders;
}

PyMODINIT_FUNC PyInit_primes() {
  static PyMethodDef methods[] = {
      {
          .ml_name = "factor_out",
          .ml_meth = factor_out,
          .ml_flags = METH_VARARGS,
          .ml_doc = "prime factorization of an integer"
      },
      {NULL, NULL, 0, NULL}
  };

  static PyModuleDef module_def = {
      .m_base = PyModuleDef_HEAD_INIT,
      .m_name = "primes",
      .m_size = -1,
      .m_methods = methods
  };



  return PyModule_Create(&module_def);
}

int main() {
  PyImport_AppendInittab("primes", PyInit_primes);
  Py_Initialize();
}
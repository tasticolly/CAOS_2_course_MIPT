#include <Python.h>

static int64_t AtPosOrZero(PyObject* matrix, int32_t i, int32_t j) {
  Py_ssize_t matrix_size = PyList_Size(matrix);
  if (i < matrix_size && j < matrix_size) {
    return PyLong_AsLong(PyList_GetItem(PyList_GetItem(matrix, i), j));
  }

  return 0;
}

static PyObject* dot(PyObject* self, PyObject* args) {
  int32_t matrix_size;
  PyObject* A;
  PyObject* B;
  if (!PyArg_ParseTuple(args, "i|O|O", &matrix_size, &A, &B)) {
    // Invalid arguments
  }

  PyObject* product = PyList_New(/*size=*/0);
  for (int32_t i = 0; i < matrix_size; ++i) {
    PyObject* current_row = PyList_New(/*size=*/0);
    for (int32_t j = 0; j < matrix_size; ++j) {
      int64_t scalar_product = 0;
      for (int32_t s = 0; s < matrix_size; ++s) {
        int64_t from_a = AtPosOrZero(A, i, s);
        int64_t from_b = AtPosOrZero(B, s, j);
        scalar_product += from_a * from_b;
      }

      PyList_Append(current_row, PyLong_FromLong(scalar_product));
    }

    PyList_Append(product, current_row);
  }

  return product;
}

PyMODINIT_FUNC PyInit_matrix() {
  static PyMethodDef methods[] = {
      {
        .ml_name = "dot",
        .ml_meth = dot,
        .ml_flags = METH_VARARGS,
        .ml_doc = "Multiply 2x2 matrices"
      },
      {NULL, NULL, 0, NULL}
  };

  static PyModuleDef module_def = {
      .m_base = PyModuleDef_HEAD_INIT,
      .m_name = "matrix",
      .m_size = -1,
      .m_methods = methods
  };

  return PyModule_Create(&module_def);
}

int main() {
  PyImport_AppendInittab("matrix", PyInit_matrix);
  Py_Initialize();
}
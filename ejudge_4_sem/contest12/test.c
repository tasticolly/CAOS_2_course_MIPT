#include <Python.h>

int main() {
  Py_Initialize();
  PyRun_SimpleString("a=1\nb=2\nprint(a+b)");
  Py_Finalize();
}
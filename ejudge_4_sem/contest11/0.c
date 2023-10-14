#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef double (* lib_fn_t)(double);

int main(int argc, char** argv) {
  const char* lib_filename = argv[1];
  const char* fn_name = argv[2];

  void* lib = dlopen(lib_filename, RTLD_NOW);
  if (lib == NULL) {
    fprintf(stderr, "%s", dlerror());
    return EXIT_FAILURE;
  }

  lib_fn_t fn = dlsym(lib, fn_name);
  if (fn == NULL) {
    return EXIT_FAILURE;
  }

  double arg;
  while (scanf("%lf", &arg) != EOF) {
    printf("%.3lf\n", fn(arg));

  }
}

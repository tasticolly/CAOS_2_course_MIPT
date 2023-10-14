from distutils.core import setup, Extension

module_name = 'primes'

setup(
    name=module_name,
    ext_modules=[Extension(module_name, extra_objects=[f"{module_name}.so"])]
)
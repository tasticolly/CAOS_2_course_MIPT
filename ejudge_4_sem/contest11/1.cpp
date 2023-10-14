#include <string>
#include "interfaces.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <regex>
#include <dlfcn.h>
#include <filesystem>

struct ClassImpl {
  void* defaultConstructorPtr{nullptr};
};


struct ClassLoaderImpl {
  ClassLoaderError errorState{ClassLoaderError::NoError};
  std::unordered_map<std::string, AbstractClass*> loadedClasses;
};


AbstractClass::AbstractClass() {
  pImpl = new ClassImpl();
}

AbstractClass::~AbstractClass() {
  delete pImpl;
}

ClassLoader::ClassLoader() {
  pImpl = new ClassLoaderImpl();
}
ClassLoader::~ClassLoader() {
  delete pImpl;
}

ClassLoaderError ClassLoader::lastError() const {
  return pImpl->errorState;
}

std::string getMangledConstructorName(const std::string& name) {
  std::string mangledName;
  mangledName += "_ZN";
  int curr_idx = 0;
  std::string tmp;
  while (curr_idx < name.size()) {
    if (name[curr_idx] != ':') {
      tmp += name[curr_idx];
    } else {
      mangledName += std::to_string(tmp.size()) + tmp;
      tmp.clear();
      ++curr_idx;
    }
    ++curr_idx;
  }
  mangledName += std::to_string(tmp.size()) + tmp;
  tmp.clear();
  mangledName += "C1Ev";
  return mangledName;
}

AbstractClass* ClassLoader::loadClass(const std::string& fullyQualifiedName) {
  if (pImpl->loadedClasses.find(fullyQualifiedName) != pImpl->loadedClasses.end()) {
    return pImpl->loadedClasses[fullyQualifiedName];
  }
  std::string classPath = std::getenv("CLASSPATH");
  std::string fileName = classPath + '/' + std::regex_replace(fullyQualifiedName, std::regex("::"), "/") + ".so";
  if (!std::filesystem::exists(fileName)) {
    pImpl->errorState = ClassLoaderError::FileNotFound;
    return nullptr;
  }
  void* lib = dlopen(fileName.data(), RTLD_NOW);

  if (lib == NULL) {
    pImpl->errorState = ClassLoaderError::LibraryLoadError;
    return nullptr;
  }

  std::string constructorName = getMangledConstructorName(fullyQualifiedName);
  void* constructor_func = dlsym(lib, constructorName.data());
  if (constructor_func == NULL) {
    pImpl->errorState = ClassLoaderError::NoClassInLibrary;
    return nullptr;
  }

  AbstractClass* abstractClassPtr = new AbstractClass();
  abstractClassPtr->pImpl->defaultConstructorPtr = constructor_func;
  pImpl->loadedClasses[fullyQualifiedName] = abstractClassPtr;
  return abstractClassPtr;
}

void* AbstractClass::newInstanceWithSize(size_t sizeofClass) {
  using constructor_t = void (*)(void*);
  void* instancePtr = malloc(sizeofClass);
  ((constructor_t) pImpl->defaultConstructorPtr)(instancePtr);
  return instancePtr;
}

#pragma once

#include <iostream>
#include <cstdlib>
#include <dlfcn.h>

using PrimeCountFunc = int (*)(int, int);
using PiFunc = float (*)(int);

void* LoadLibrary(const char *libraryName);
void UnloadLibrary(void* handle);
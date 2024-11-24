#pragma once

#include <iostream>
#include <cstdlib>
#include <dlfcn.h>

using SquareFunc = float (*)(float, float);
using EFunc = float (*)(int);

void* LoadLibrary(const char *libraryName);
void UnloadLibrary(void* handle);
#include "implementations.hpp"
#include <cmath>

extern "C" float Square(float A, float B) {
    return A * B;
}

extern "C" float E(int x) {
    return pow(1.0 + 1.0 / x, x);
}
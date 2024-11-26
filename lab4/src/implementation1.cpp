#include "implementations.hpp"
#include <cmath>

extern "C" float Square(float A, float B) {
    if ((A <= 0) || (B <= 0)){
        return -1;
    }
    return A * B;
}

extern "C" float E(int x) {
    if (x <= 0){
        return -1;
    }
    return pow(1.0 + 1.0 / x, x);
}
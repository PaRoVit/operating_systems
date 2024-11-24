#include "implementations.hpp"

extern "C" float Square(float A, float B) {
    return A * B * 0.5;
}

extern "C" float Factorial(int n) {
    if (n == 0 || n == 1) return 1.0;
    float result = 1.0;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

extern "C" float E(int x) {
    double e = 0.0;
    for (int n = 0; n <= x; ++n) {
        e += 1.0 / Factorial(n);
    }
    return e;
}
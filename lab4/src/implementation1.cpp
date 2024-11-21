#include "implementations.hpp"

extern "C" bool IsPrime(int number) {
    if (number < 2) return false; // Числа меньше 2 не являются простыми
    for (int i = 2; i <= sqrt(number); ++i) {
        if (number % i == 0) {
            return false; // Если делится на i, то не простое
        }
    }
    return true; // Простое число
}

extern "C" int PrimeCount(int A, int B) {
    int count = 0;
    for (int i = A; i <= B; ++i) {
        if (IsPrime(i)) {
            ++count;
        }
    }
    return count;
}

extern "C" float Pi(int K) {
    float pi = 0.0f; // Инициализируем результат
    for (int i = 0; i < K; ++i) {
        // Чередуем знаки и вычисляем текущий член ряда
        float term = 1.0f / (2 * i + 1);
        if (i % 2 == 0) {
            pi += term; // Добавляем, если чётный индекс
        } else {
            pi -= term; // Вычитаем, если нечётный индекс
        }
    }
    return 4 * pi; // Умножаем сумму на 4 для получения значения π
}
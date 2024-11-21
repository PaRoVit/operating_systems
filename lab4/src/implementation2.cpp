#include "implementations.hpp"

extern "C" int PrimeCount(int A, int B) {
    if (B < 2) return 0; // Нет простых чисел меньше 2
    
    // Создаём массив для решета до числа B
    int limit = B + 1;
    std::vector<bool> is_prime(limit, true);
    is_prime[0] = is_prime[1] = false; // 0 и 1 не являются простыми

    // Решето Эратосфена: помечаем составные числа
    for (int i = 2; i <= std::sqrt(B); ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j < limit; j += i) {
                is_prime[j] = false;
            }
        }
    }

    // Подсчитываем количество простых чисел в диапазоне [A, B]
    int count = 0;
    for (int i = std::max(2, A); i <= B; ++i) {
        if (is_prime[i]) {
            ++count;
        }
    }

    return count;
}

extern "C" float Pi(int K) {
    if (K <= 0) return 0.0f; // Если K <= 0, возвращаем 0

    float pi = 1.0f; // Начальное значение произведения
    for (int n = 1; n <= K; ++n) {
        float term = (4.0f * n * n) / (4.0f * n * n - 1.0f);
        pi *= term; // Умножаем очередной член ряда
    }
    return 2.0f * pi; // Умножаем результат на 2
}
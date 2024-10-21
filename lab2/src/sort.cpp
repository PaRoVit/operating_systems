#include <sort.hpp>

void oddEvenSort(int arr[], int start, int end) {
    bool isSorted = false; 

    while (!isSorted) {
        isSorted = true;

        // Четная фаза: сортировка четных индексов
        for (int i = start + 1; i <= end - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                isSorted = false;
            }
        }

        // Нечетная фаза: сортировка нечетных индексов
        for (int i = start; i <= end - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                isSorted = false;
            }
        }
    }
}

// Функция, которая будет выполняться в потоке (для pthread)
void* thread_function(void* args) {
    ThreadArguments* threadArgs = (ThreadArguments*) args;
    oddEvenSort(threadArgs->array, threadArgs->start, threadArgs->end);
    return nullptr;
}

void createAndRunThreads(std::vector<pthread_t>& threads, std::vector<ThreadArguments>& thread_args, int* array, int arraySize, int threadsAmount) {
    int pieceSize = arraySize / threadsAmount;
    int left = arraySize % threadsAmount;

    int startIndex = 0;
    int endIndex = pieceSize - 1;

    for (int iteration = 0; iteration < threadsAmount; ++iteration) {
        if (left > 0) {
            endIndex++;
            left--;
        }

        thread_args[iteration].array = array;
        thread_args[iteration].start = startIndex;
        thread_args[iteration].end = endIndex;

        // Создание потоков с использованием pthread_create
        pthread_create(&threads[iteration], nullptr, thread_function, &thread_args[iteration]);

        startIndex = endIndex + 1;
        endIndex = startIndex + pieceSize - 1;
    }
}

void creatArray(int* array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = size - i;
    }
}

void waitThreads(std::vector<pthread_t>& threads) {
    for (auto& thread : threads) {
        // Ожидание завершения каждого потока с использованием pthread_join
        pthread_join(thread, nullptr);
    }
}

std::vector<int> run(int threadsAmount) {

    int ArrayForSort[AMOUNT_OF_ELEMENTS];
    int arraySize = sizeof(ArrayForSort) / sizeof(ArrayForSort[0]);

    // Заполнение массива
    creatArray(ArrayForSort, arraySize);

    std::vector<pthread_t> threads(threadsAmount);
    std::vector<ThreadArguments> thread_args(threadsAmount);

    createAndRunThreads(threads, thread_args, ArrayForSort, arraySize, threadsAmount);

    waitThreads(threads);

    // Финальная сортировка всего массива
    oddEvenSort(ArrayForSort, 0, arraySize - 1);

    return std::vector<int>(ArrayForSort, ArrayForSort + arraySize);

}
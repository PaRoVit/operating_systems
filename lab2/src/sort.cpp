#include <sort.hpp>

void OddEvenSort(int arr[], int start, int end) {
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
void* ThreadFunction(void* args) {
    ThreadArguments* threadArgs = (ThreadArguments*) args;
    OddEvenSort(threadArgs->array, threadArgs->start, threadArgs->end);
    return nullptr;
}

void CreateAndRunThreads(std::vector<pthread_t>& threads, std::vector<ThreadArguments>& thread_args, int* array, int arraySize, int threadsAmount) {
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
        pthread_create(&threads[iteration], nullptr, ThreadFunction, &thread_args[iteration]);

        startIndex = endIndex + 1;
        endIndex = startIndex + pieceSize - 1;
    }
}

void CreatArray(int* array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = size - i;
    }
}

void WaitThreads(std::vector<pthread_t>& threads) {
    for (auto& thread : threads) {
        // Ожидание завершения каждого потока с использованием pthread_join
        pthread_join(thread, nullptr);
    }
}

std::vector<int> RunMain(int threadsAmount, int arraySize) {

    std::vector<int> ArrayForSort(AMOUNT_OF_ELEMENTS);

    // Заполнение массива
    CreatArray(ArrayForSort.data(), arraySize);

    std::vector<pthread_t> threads(threadsAmount);
    std::vector<ThreadArguments> thread_args(threadsAmount);

    CreateAndRunThreads(threads, thread_args, ArrayForSort.data(), arraySize, threadsAmount);

    WaitThreads(threads);

    // Финальная сортировка всего массива
    OddEvenSort(ArrayForSort.data(), 0, arraySize - 1);

    return ArrayForSort;

}
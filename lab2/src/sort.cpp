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

// Функция, которая будет выполняться в потоке
void thread_function(ThreadArguments* args) {
    oddEvenSort(args->array, args->start, args->end);
}


void createAndRunThreads(std::vector<std::thread>& threads, std::vector<ThreadArguments>& thread_args, int* array, int arraySize, int threadsAmount) {
    // Определяем размер части массива, которую будет сортировать каждый поток
    int pieceSize = arraySize / threadsAmount;
    // Остаток от деления массива на части, если он не делится ровно на потоки
    int left = arraySize % threadsAmount;

    // Инициализируем стартовый индекс для первой части массива
    int startIndex = 0;
    // Конечный индекс первой части массива (на основе размера части)
    int endIndex = pieceSize - 1;

    // создаём потоки 
    for (int iteration = 0; iteration < threadsAmount; ++iteration) {
        if (left > 0) {
            endIndex++;
            left--;
        }
        // передаём ссылки на массив и диапазоны сортировки 
        thread_args[iteration].array = array;
        thread_args[iteration].start = startIndex;
        thread_args[iteration].end = endIndex;
        // создание потоков
        threads.emplace_back(thread_function, &thread_args[iteration]);
        // обновление начального и конечного индекса
        startIndex = endIndex + 1;
        endIndex = startIndex + pieceSize - 1;
    }
}

void creatArray(int* array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = size - i;
    }
}

void waitThreads(std::vector<std::thread>& threads) {
    for (auto& thread : threads) {
        thread.join();
    }
}

std::vector<int> run(int threadsAmount) {
    // std::cout << "Amount_of_threads is " << threadsAmount << "\n";

    int ArrayForSort[AMOUNT_OF_ELEMENTS];
    int arraySize = sizeof(ArrayForSort) / sizeof(ArrayForSort[0]);

    // Заполнение массива
    creatArray(ArrayForSort, arraySize);

    // auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    std::vector<ThreadArguments> thread_args(threadsAmount);

    createAndRunThreads(threads, thread_args, ArrayForSort, arraySize, threadsAmount);

    waitThreads(threads);

    // Финальная сортировка всего массива (съедает слишком много времени)
    oddEvenSort(ArrayForSort, 0, arraySize - 1);
    // parallel_merge(ArrayForSort, arraySize, threadsAmount);

    // auto end = std::chrono::high_resolution_clock::now();

    // double execution_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // std::cout << "Execution_time: " << execution_time << " ms\n";

    // Раскомментировать для вывода массива
    // for (int iteration = 0; iteration < arraySize; iteration++) {
    //     std::cout << ArrayForSort[iteration] << ", ";
    // }
    // std::cout << "\n";


    return std::vector<int>(ArrayForSort, ArrayForSort + arraySize);
}
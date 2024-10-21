#pragma once

#include <iostream>
#include <thread>
#include <vector>
#include <sys/time.h>
#include <algorithm>
#include <pthread.h>

constexpr int AMOUNT_OF_ELEMENTS = 10000;

struct ThreadArguments {
    int* array;
    int start;
    int end;
};

void oddEvenSort(int arr[], int start, int end);
void* thread_function(void* args);
void createAndRunThreads(std::vector<pthread_t>& threads, std::vector<ThreadArguments>& thread_args, int* array, int arraySize, int threadsAmount);
void creatArray(int* array, int size);
void waitThreads(std::vector<pthread_t>& threads);
std::vector<int> run(int threadsAmount);

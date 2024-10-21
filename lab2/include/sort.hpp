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

void OddEvenSort(int arr[], int start, int end);
void* ThreadFunction(void* args);
void CreateAndRunThreads(std::vector<pthread_t>& threads, std::vector<ThreadArguments>& thread_args, int* array, int arraySize, int threadsAmount);
void CreatArray(int* array, int size);
void WaitThreads(std::vector<pthread_t>& threads);
std::vector<int> RunMain(int threadsAmount, int arraySize);

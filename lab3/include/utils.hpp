#pragma once

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>     
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>

std::string removeVowels(const std::string& input);

sem_t* CreateSemaphore(const char *name, int value);
int CreateShm(const char* name);
char* MapSharedMemory(const int size, int fd);
int CreateFork();
void ErrorChecking(int result, const char* error);
int GetSemaphoreValue(sem_t* semaphore);
void SetSemaphoreValue(sem_t* semaphore, int value);
void ProcessChild(const char *semaphoreName, const char* mmapFilename);


constexpr const char *SEM_NAME = "SEM";

constexpr const char* MMAP_NAME1 = "/shm1";
constexpr const char* MMAP_NAME2 = "/shm2";

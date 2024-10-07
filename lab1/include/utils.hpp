#pragma once

#include <sstream>
#include <algorithm>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <ext/stdio_filebuf.h>
#include <fcntl.h>

std::string removeVowels(const std::string& input);

ssize_t readStringFromPipe(int pipeFd, std::string &input_string);

ssize_t writeStringToPipe(int pipeFd, const std::string &output_string);

enum PipeEnd {
    READ_END,
    WRITE_END
};

void                CreatePipe(int pipeFd[2]);
pid_t               CreateChild();
void                Exec(const char * pathToChild);
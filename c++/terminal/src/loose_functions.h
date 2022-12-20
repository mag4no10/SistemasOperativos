#pragma once

#include <iostream>      //Basic input and output
#include <vector>        //std::vector
#include <limits.h>      //LOGIN_NAME_MAX, PATH_MAX
#include <libgen.h>      //dirname, basename
#include <unistd.h>      //write, read


inline bool preserve_all = false;

std::string dirname(std::string&);
std::string basename_(std::string&);

std::error_code print(const std::string&);
std::error_code read(int fd, std::vector<uint8_t>&);
std::error_code write(int fd, std::vector<uint8_t>&);
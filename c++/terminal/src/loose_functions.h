#pragma once

#include <iostream>


inline bool preserve_all = false;

void usage(int, char* []);
void information();

std::string dirname(std::string&);
std::string basename_(std::string&);

std::error_code print(const std::string&);
std::error_code read(int fd, std::vector<uint8_t>&);
std::error_code write(int fd, std::vector<uint8_t>&);
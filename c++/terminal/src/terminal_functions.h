#pragma once

#include <iostream>
#include <system_error>
#include <vector>

inline std::vector<uint8_t> pending_input(0);
inline std::string linea;
std::error_code print(const std::string&);
std::error_code read(int, std::vector<uint8_t>&);
void print_prompt(const int&);
std::error_code read_line(int, std::string&);

#pragma once

#include <iostream>
#include <system_error>
#include <vector>

inline std::vector<uint8_t> pending_input(0);
inline std::string linea;
void print_prompt(const int&);
std::error_code read_line(int, std::string&);

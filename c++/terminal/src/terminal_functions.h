#pragma once

#include <iostream>
#include <system_error>
#include <vector>

inline std::vector<uint8_t> pending_input(0);
std::error_code read_line(int, std::string&);
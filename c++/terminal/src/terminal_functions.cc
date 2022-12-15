#include <iostream>
#include <system_error>
#include <vector>
#include <string>
#include <algorithm>

#include "terminal_functions.h"

std::error_code read_line(int fd, std::string& line) {
    int counter{0};
    for (const uint8_t i: pending_input) {
        if (i == '\n') {
            std::copy(pending_input.begin(), pending_input.begin() + counter, line.begin());
            pending_input.erase(pending_input.begin(),pending_input.begin()+counter);
            return std::error_code(0, std::system_category());
        }
        counter++;
    }
    return std::error_code(0, std::system_category());
}
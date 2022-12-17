#include <iostream>
#include <vector>
#include <limits.h>
#include <libgen.h>
#include <unistd.h>

#include "loose_functions.h"

void usage(int argc, char* argv[]) {
    
}

void information() {

}

std::string dirname(std::string& path) {
    std::vector<char> buffer(PATH_MAX);
    path.copy(buffer.data(), buffer.size());
    std::string dir = dirname(buffer.data());
    buffer.clear();
    return dir;
}

std::string basename_(std::string& path) {
    std::vector<char> buffer(PATH_MAX);
    path.copy(buffer.data(), buffer.size());
    std::string file_name = basename(buffer.data());
    buffer.clear();
    return file_name;
}

std::error_code print(const std::string& str) {
    uint32_t bytes_written = write(STDOUT_FILENO, str.c_str(), str.size());
    if (bytes_written < 0) {
        //Manejar error
    }
    return std::error_code(0, std::system_category());
}

std::error_code read(int fd, std::vector<uint8_t>& buffer) {
    uint32_t bytes_read = read(fd, buffer.data(), buffer.size());
    if (bytes_read < 0) {
        return std::error_code(EIO, std::system_category());
    }
    buffer.resize(bytes_read);
    return std::error_code(0, std::system_category());
}

std::error_code write(int fd, std::vector<uint8_t>& buffer) {
    uint32_t bytes_written = write(fd, buffer.data(), buffer.size());
    if (bytes_written < 0) {
        return std::error_code(EIO, std::system_category());
    }
    if (bytes_written < buffer.size()) {
        uint32_t bytes_left = buffer.size() - bytes_written;
        bytes_written = write(fd, buffer.data() + bytes_written, bytes_left);
        if (bytes_written < buffer.size()) { 
            return std::error_code(ENOMEM, std::system_category());
        }
    }
    return std::error_code(0, std::system_category());
}

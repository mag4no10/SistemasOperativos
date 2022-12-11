#pragma once

#include <iostream>
#include <vector>

std::error_code copy_file(
    const std::string& src_path, 
    std::string& dst_path,
    bool preserve_all
);

std::error_code move_file(
    const std::string& src_path, 
    std::string& dst_path
);

std::error_code read(
    int fd, 
    std::vector<uint8_t>& buf
);

std::error_code write(
    int fd, 
    std::vector<uint8_t>& buf
);
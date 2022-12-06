#pragma once

#include <iostream>
#include <vector>

void copy_file(
    const std::string& src_path, 
    std::string dst_path,
    bool preserve_all
);

void move_file(
    const std::string& src_path, 
    const std::string& dst_path
);

std::string getenv(
    const std::string& name
);

int read(
    int fd, 
    std::vector<uint8_t>& buf
);

int write(
    int fd, 
    std::vector<uint8_t>& buf
);

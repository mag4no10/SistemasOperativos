#pragma once

#include <iostream>
#include <vector>

void copy_file(
    const std::string& src_path, 
    const std::string& dst_path,
    bool preserve_all=false
);

void move_file(
    const std::string& src_path, 
    const std::string& dst_path
);

std::string getenv(
    const std::string& name
);

int reads(
    int fd, 
    std::vector<uint8_t>& buf
);

ssize_t write(
    int fd, 
    const void* buf, 
    size_t count
);

/*
int open(
    const char* pathname, 
    int flags
);

int open(
    const char* pathname,
    int flags,
    mode_t mode
);
*/
#include <iostream>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <utime.h>
#include <limits.h>

#include "functions.h"

std::string dirname(const std::string& path) {
    std::vector<char> buffer(PATH_MAX);
    path.copy(buffer.data(), buffer.size());
    std::string dir = dirname(buffer.data());
    buffer.clear();
    return dir;
}

std::string basename(const std::string& path) {
    std::vector<char> buffer(PATH_MAX);
    path.copy(buffer.data(), buffer.size());
    std::string file_name = basename(buffer.data());
    buffer.clear();
    return file_name;
}

void copy_file(const std::string& src_path, std::string dst_path, bool preserve_all=false) {
    struct stat src_st, dst_st;
    if (stat(src_path.c_str(), &src_st) == -1) {
        //error
    }
    if (!S_ISREG(src_st.st_mode)) {
        //error
    }
    if (stat(dst_path.c_str(), &dst_st) != -1 ) {
        if (src_st.st_dev == dst_st.st_dev && src_st.st_ino == dst_st.st_ino) {
            //error
        }
        if (S_ISDIR(dst_st.st_mode)) {
            dst_path = dirname(dst_path) + basename(src_path);
        }
    }

    std::vector<uint8_t> buffer(16ul * 1024 * 1024);

    int src_fd = open(src_path.c_str(),O_RDONLY);
    if (src_fd < 0) {
        //error
    }
    int dst_fd = open(dst_path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (dst_fd < 0) {
        //error
    }

    ssize_t bytes_left = src_st.st_size;
    while (bytes_left > 0) {
        read(src_fd, buffer);
        write(dst_fd, buffer);
        bytes_left = bytes_left - buffer.size();
        buffer.clear();
    }
    
    close(src_fd);
    close(dst_fd);

    if (preserve_all) {
        mode_t src_file_permission = src_st.st_mode & ~S_IFMT;
        chmod(dst_path.c_str(), src_file_permission);

        uid_t src_file_owner = src_st.st_uid;
        uid_t src_file_group = src_st.st_gid;
        chown(dst_path.c_str(), src_file_owner, src_file_group);

        time_t src_last_access = src_st.st_atime;
        time_t src_last_modification = src_st.st_mtime;
        struct utimbuf src_uti;
        src_uti.actime = src_last_access;
        src_uti.modtime = src_last_modification;
        utime(dst_path.c_str(), &src_uti);
    }
}

void move_file(const std::string& src_path, const std::string& dst_path) {

}

std::string getenv(const std::string& name) {
    char* value = getenv(name.c_str());
    if (value) {
        return std::string(value);
    }
    else {
        return std::string();
    }
}

int read(int fd, std::vector<uint8_t>& buffer) {
    ssize_t bytes_read = read(fd, buffer.data(), buffer.size());
    if (bytes_read < 0) {
        //codigo error
    }
    buffer.resize(bytes_read);
    return 0;
}

int write(int fd, std::vector<uint8_t>& buffer) {
    ssize_t bytes_written = write(fd, buffer.data(), buffer.size());
    if (bytes_written < buffer.size()) {
        ssize_t data_not_written = buffer.size() - bytes_written;
        ssize_t bytes_written = bytes_written + write(fd, buffer.data(), buffer.size());
    }
    return 0;
}
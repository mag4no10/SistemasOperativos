#include <iostream>
#include <vector>
#include <sys/stat.h>     // stat
#include <unistd.h>       // close
#include <fcntl.h>        // open
#include <utime.h>        // utime
#include <system_error>   // class error

#include "copy_move.h"
#include "loose_functions.h"
#include "scope.hpp"


std::error_code copy_file(const std::string& src_path, std::string& dst_path, bool preserve_all=false) {
    struct stat src_st, dst_st;
    if (stat(src_path.c_str(), &src_st) == -1) {
        // error si src_path no existe
        return std::error_code(errno, std::system_category());
    }
    if (!S_ISREG(src_st.st_mode)) {
        // error si src_path no es un archivo regular
        return std::error_code(errno, std::system_category());
    }
    if (stat(dst_path.c_str(), &dst_st) != -1 ) {
        if (src_st.st_dev == dst_st.st_dev && src_st.st_ino == dst_st.st_ino) {
            // error si src_path y dst_path son el mismo archivo dentro
            // del mismo dispositivo
            return std::error_code(errno, std::system_category());
        }
        if (S_ISDIR(dst_st.st_mode)) {
            // si dst_path es un directorio, dst_path es dst_path + basename de src_path 
            std::string dst_path_copy(dst_path), src_path_copy(src_path);
            dst_path = dirname(dst_path_copy) + "/" + basename_(dst_path_copy) + "/" + basename_(src_path_copy);
        }
    }

    std::vector<uint8_t> buffer(16ul * 1024 * 1024);

    int src_fd = open(src_path.c_str(), O_RDONLY);
    if (src_fd < 0) {
        // error si no se puede abrir src_path
        return std::error_code(errno, std::system_category());
    }
    auto src_guard = scope::scope_exit(
        [src_fd] { close(src_fd); }
    );
    int dst_fd = open(dst_path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (dst_fd < 0) {
        // error si no se puede abrir dst_path
        return std::error_code(errno, std::system_category());
    }
    auto dst_guard = scope::scope_exit(
        [dst_fd] { close(dst_fd); }
    );
    
    ssize_t bytes_left = src_st.st_size;
    ssize_t bytes_leidos{0};
    while (bytes_left >= 0) {
        bool error1 = read(src_fd, buffer);
        if (error1) {
            return std::error_code(errno, std::system_category());
        }
        bytes_leidos = bytes_leidos + buffer.size();
        bool error2 = write(dst_fd, buffer);
        if (error2) {
            return std::error_code(errno, std::system_category());
        }
        bytes_left = bytes_left - bytes_leidos;
        buffer.clear();
        if (bytes_left <= 0) {
            break;
        }
        buffer.resize(bytes_left);
    }
    
    close(src_fd);
    close(dst_fd);

    if (preserve_all) {
        mode_t src_file_permission = src_st.st_mode & ~S_IFMT;
        int error3 = chmod(dst_path.c_str(), src_file_permission);
        if (error3 == -1) {
            return std::error_code(errno, std::system_category());
        }

        uid_t src_file_owner = src_st.st_uid;
        uid_t src_file_group = src_st.st_gid;
        int error4 = chown(dst_path.c_str(), src_file_owner, src_file_group);
        if (error4 == -1) {
            return std::error_code(errno, std::system_category());
        }

        time_t src_last_access = src_st.st_atime;
        time_t src_last_modification = src_st.st_mtime;
        struct utimbuf src_uti;
        src_uti.actime = src_last_access;
        src_uti.modtime = src_last_modification;
        int error5 = utime(dst_path.c_str(), &src_uti);
        if (error5 == -1) {
            return std::error_code(errno, std::system_category());
        }
    }
    return std::error_code(0, std::system_category());
}

std::error_code move_file(const std::string& src_path, std::string& dst_path) {
    struct stat src_st, dst_st;
    if (stat(src_path.c_str(), &src_st) == -1) {
        // error si src_path no existe
        return std::error_code(errno, std::system_category());
    }
    if (stat(dst_path.c_str(), &dst_st) == -1) {
        // error si dst_path no existe
        return std::error_code(errno, std::system_category());
    }
    if (S_ISDIR(dst_st.st_mode)) {
        // si dst_path es un directorio, dst_path es dst_path + basename de src_path 
        std::string dst_path_copy(dst_path), src_path_copy(src_path);
        dst_path = dirname(dst_path_copy) + "/" + basename_(dst_path_copy) + "/" + basename_(src_path_copy);    
    }
    if (src_st.st_dev == dst_st.st_dev && src_st.st_ino == dst_st.st_ino) {
        // si src_path y dst_path son el mismo archivo dentro
        // del mismo dispositivo, dst_path se renombra a src
        int error1 = rename(dst_path.c_str(), src_path.c_str());
        if (error1 == -1) {
            return std::error_code(errno, std::system_category());
        }
        return std::error_code(0, std::system_category());;
    }
    bool preserve;
    std::error_code error2 = copy_file(src_path, dst_path, preserve = true);
    if (error2) {
        return std::error_code(errno, std::system_category());
    }
    int error3 = unlink(src_path.c_str());
    if (error3 == -1) {
        return std::error_code(errno, std::system_category());
    }

    return std::error_code(0, std::system_category());
}

int read(int fd, std::vector<uint8_t>& buffer) {
    uint32_t bytes_read = read(fd, buffer.data(), buffer.size());
    if (bytes_read < 0) {
        return 1;
    }
    buffer.resize(bytes_read);
    return 0;
}

int write(int fd, std::vector<uint8_t>& buffer) {
    uint32_t bytes_written = write(fd, buffer.data(), buffer.size());
    if (bytes_written < 0) {
        return 1;
    }
    if (bytes_written < buffer.size()) {
        uint32_t bytes_left = buffer.size() - bytes_written;
        bytes_written = write(fd, buffer.data(), bytes_left);
        if (bytes_written < buffer.size()) { 
            return 1;
        }
    }
    return 0;
}
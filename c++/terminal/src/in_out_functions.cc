#include <iostream>
#include <vector>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <utime.h>
#include <fcntl.h>
#include <string>

#include "in_out_functions.h"
#include "loose_functions.h"
#include "scope.hpp"


int echo_command(const std::vector<std::string>& args) {
    std::string output;
    int counter{0};
    for (std::string i : args) {
        if (counter == 0) {
            counter++;
            continue;
        }
        output = output + i + " ";
        counter++;
    }
    std::cout << output << std::endl;
    return 0;
}

int cd_command(const std::vector<std::string>& args) {
    if (args.size() > 2) {
        //error demasiados argumentos
    }
    std::string path = args.at(1);
    if (path.size() > PATH_MAX) {
        //error tamaño maximo
    }
    chdir(path.c_str());
    return 0;
}

int cp_command(const std::vector<std::string>& args) {
    if (args.size() > 4 || args.size() < 2) {
        //error numero de argumentos invalido
    }
    std::string mode(""), src_path = args.at(1), dst_path = args.at(2);
    if (src_path == "-m" || src_path == "-a") {
        mode     = args.at(1);
        src_path = args.at(2);
        dst_path = args.at(3);
        preserve_all = true;
    }
    struct stat src_st, dst_st;
    if (stat(src_path.c_str(), &src_st) == -1) {
        // error si src_path no existe
    }
    if (!S_ISREG(src_st.st_mode)) {
        // error si src_path no es un archivo regular
    }
    if (stat(dst_path.c_str(), &dst_st) != -1 ) {
        if (src_st.st_dev == dst_st.st_dev && src_st.st_ino == dst_st.st_ino) {
            // error si src_path y dst_path son el mismo archivo dentro
            // del mismo dispositivo
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
    }
    auto src_guard = scope::scope_exit(
        [src_fd] { close(src_fd); }
    );
    int dst_fd = open(dst_path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (dst_fd < 0) {
        // error si no se puede abrir dst_path
    }
    auto dst_guard = scope::scope_exit(
        [dst_fd] { close(dst_fd); }
    );
    ssize_t bytes_left = src_st.st_size;
    ssize_t bytes_leidos{0};
    while (bytes_left >= 0) {
        std::error_code error1 = read(src_fd, buffer);
        if (error1) {
            //return error1
        }
        bytes_leidos = bytes_leidos + buffer.size();
        std::error_code error2 = write(dst_fd, buffer);
        if (error2) {
            //return error2;
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
            //error chmod
        }

        uid_t src_file_owner = src_st.st_uid;
        uid_t src_file_group = src_st.st_gid;
        int error4 = chown(dst_path.c_str(), src_file_owner, src_file_group);
        if (error4 == -1) {
            //error chown
        }

        time_t src_last_access = src_st.st_atime;
        time_t src_last_modification = src_st.st_mtime;
        struct utimbuf src_uti;
        src_uti.actime = src_last_access;
        src_uti.modtime = src_last_modification;
        int error5 = utime(dst_path.c_str(), &src_uti);
        if (error5 == -1) {
            //error utime
        }
    }  
     
    return 0;
}

int mv_command(const std::vector<std::string>& args) {
    std::string mode(""), src_path = args.at(1), dst_path = args.at(2);
    preserve_all = true;
    struct stat src_st, dst_st;
    if (stat(src_path.c_str(), &src_st) == -1) {
        // error si src_path no existe
    }
    if (stat(dst_path.c_str(), &dst_st) == -1) {
        // error si dst_path no existe
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
            //error rename
        }
        return 0;
    }
    int error2 = cp_command(args);
    if (error2 == 1) {
        //error cp_command
    }

    int error3 = unlink(src_path.c_str());
    if (error3 == -1) {
        //error unlink
    }

    return 0;
}

int clear_command() {
    if (OS_Windows == 0) {
        std::system("clear");
    }
    else if (OS_Windows == 1) {
        std::system("cls");
    }
    else {
        std::cout << "Operating system not supported" << std::endl;
    }
    return 0;
}

int execute_program(const std::vector<std::string>& args, bool has_wait=true) {
    std::vector<const char*> argv;
    for(std::string i : args) {
        argv.push_back(i.c_str());
    }
    argv.push_back(NULL);
    int status_code = execvp(argv[0], const_cast<char* const*>(argv.data()));
    if (status_code < 0) {
        return 1;
    }
    return 0;
}
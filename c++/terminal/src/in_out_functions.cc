#include "in_out_functions.h"
#include "loose_functions.h"
#include "scope.hpp"


std::error_code echo_command(const std::vector<std::string>& args) {
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
    output = output + '\n';
    std::cout << output;
    return std::error_code(0, std::system_category());
}

std::error_code cd_command(const std::vector<std::string>& args) {
    if (args.size() > 2) {
        perror("Demasiados argumentos");
        return std::error_code(E2BIG, std::system_category());
    }
    std::string path_to_cd;
    if (args.size() == 1) {
        char username[LOGIN_NAME_MAX];
        getlogin_r(username, LOGIN_NAME_MAX);
        path_to_cd = "/home/" + std::string(username);
    }
    else {
        path_to_cd = args.at(1);
    }
    struct stat path;
    if (stat(path_to_cd.c_str(), &path) == -1) {
        return std::error_code(ENOENT, std::system_category());
    }
    if (path_to_cd.size() > PATH_MAX) {
        return std::error_code(EOVERFLOW, std::system_category());
    }
    chdir(path_to_cd.c_str());
    return std::error_code(0, std::system_category());
}

std::error_code cp_command(const std::vector<std::string>& args) {
    if (args.size() > 4 || args.size() < 2) {
        std::cerr << "Invalid number of arguments" << std::endl;
        return std::error_code(EDOM, std::system_category());
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
        return std::error_code(ENOENT, std::system_category());
    }
    if (!S_ISREG(src_st.st_mode)) {
        return std::error_code(EACCES, std::system_category());
    }
    if (stat(dst_path.c_str(), &dst_st) != -1 ) {
        if (src_st.st_dev == dst_st.st_dev && src_st.st_ino == dst_st.st_ino) {
            return std::error_code(EEXIST, std::system_category());
        }
        if (S_ISDIR(dst_st.st_mode)) {
            std::string dst_path_copy(dst_path), src_path_copy(src_path);
            dst_path = dirname(dst_path_copy) + "/" + basename_(dst_path_copy) + "/" + basename_(src_path_copy);
        }
    }
    std::vector<uint8_t> buffer(16ul * 1024 * 1024);

    int src_fd = open(src_path.c_str(), O_RDONLY);
    if (src_fd < 0) {
        return std::error_code(EBADF, std::system_category());
    }
    auto src_guard = scope::scope_exit(
        [src_fd] { close(src_fd); }
    );
    int dst_fd = open(dst_path.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0666);
    if (dst_fd < 0) {
        return std::error_code(EBADF, std::system_category());
    }
    auto dst_guard = scope::scope_exit(
        [dst_fd] { close(dst_fd); }
    );
    ssize_t bytes_left = src_st.st_size;
    ssize_t bytes_leidos{0};
    while (bytes_left >= 0) {
        std::error_code error1 = read(src_fd, buffer);
        if (error1) {
            return error1;
        }
        bytes_leidos = bytes_leidos + buffer.size();
        std::error_code error2 = write(dst_fd, buffer);
        if (error2) {
            return error2;
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
            perror("No se ha podido cambiar los permisos de usuario del fichero destino");
        }

        uid_t src_file_owner = src_st.st_uid;
        uid_t src_file_group = src_st.st_gid;
        int error4 = chown(dst_path.c_str(), src_file_owner, src_file_group);
        if (error4 == -1) {
            perror("No se ha podido cambiar los permisos de grupo del fichero destino");
        }

        time_t src_last_access = src_st.st_atime;
        time_t src_last_modification = src_st.st_mtime;
        struct utimbuf src_uti;
        src_uti.actime = src_last_access;
        src_uti.modtime = src_last_modification;
        int error5 = utime(dst_path.c_str(), &src_uti);
        if (error5 == -1) {
            perror("No se ha podido cambiar la ultima hora de acceso y modificacion del fichero destino");
        }
    }  
     
    return std::error_code(0, std::system_category());
}

std::error_code mv_command(const std::vector<std::string>& args) {
    std::string mode(""), src_path = args.at(1), dst_path = args.at(2);
    preserve_all = true;
    struct stat src_st, dst_st;
    if (stat(src_path.c_str(), &src_st) == -1) {
        return std::error_code(ENOENT, std::system_category());
    }
    if (stat(dst_path.c_str(), &dst_st) == -1) {
        return std::error_code(ENOENT, std::system_category());
    }
    if (S_ISDIR(dst_st.st_mode)) {
        std::string dst_path_copy(dst_path), src_path_copy(src_path);
        dst_path = dirname(dst_path_copy) + "/" + basename_(dst_path_copy) + "/" + basename_(src_path_copy);    
    }
    if (src_st.st_dev == dst_st.st_dev && src_st.st_ino == dst_st.st_ino) {
        int error1 = rename(dst_path.c_str(), src_path.c_str());
        if (error1 == -1) {
            perror("No se ha podido cambiar el nombre del fichero destino");
        }
        return std::error_code(0, std::system_category());
    }
    std::error_code error2 = cp_command(args);
    if (error2) {
        perror("Error a la hora de copiar el archivo");
    }

    int error3 = unlink(src_path.c_str());
    if (error3 != 0) {
        perror("No se ha podido borrar el archivo origen");
    }

    return std::error_code(0, std::system_category());
}

std::error_code clear_command() {
    if (OS_Windows == 0) {
        std::system("clear");
    }
    else if (OS_Windows == 1) {
        std::system("cls");
    }
    else {
        std::cout << "Operating system not supported" << std::endl;
        std::error_code(EOPNOTSUPP, std::system_category());
    }
    return std::error_code(0, std::system_category());
}
#include <iostream>
#include <vector>
#include <limits.h>    // MAX_PATH
#include <libgen.h>    // DIRNAME & BASENAME

#include "loose_functions.h"

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

std::string getenv(const std::string& name) {
    char* value = getenv(name.c_str());
    if (value) {
        return std::string(value);
    }
    else {
        return std::string();
    }
}

void information() {
    std::cout << "Este programa permite copiar o mover ficheros de una ruta hacia otra.";
    std::cout << "Modo de uso:" << std::endl;
    std::cout << "./mini_terminal [-a/-m] [/path/to/file] [/path/to/file]" << std::endl;
}

void usage(int argc, char* argv[]) {
    std::string arg1(argv[1]);
    if (arg1 == "-h" || arg1 == "--help") {
        information();
        exit(EXIT_SUCCESS);
    }
    if (argc != 4) {
        std::cout << "Numero de argumentos insuficientes, escriba " << 
        "\"./mini_terminal -h\" para imprimir el funcionamiento del sistema" << std::endl;
        exit(EXIT_FAILURE);
    }
}
#include <iostream>
#include <system_error>

#include "copy_move.h"
#include "loose_functions.h"

int main(int argc, char* argv[]) {
    usage(argc, argv);

    std::string mode     (argv[1]);
    std::string src_path (argv[2]);
    std::string dst_path (argv[3]);

    if (mode == "-m") {
        std::error_code error = move_file(src_path, dst_path);
        if (error) {
            std::cerr << "Error: " << error.message() << error.value() << std::endl;
        }
    }
    else if (mode == "-a") {
        bool preserve;
        std::error_code error = copy_file(src_path, dst_path, preserve = true);
        if (error) {
            std::cerr << "Error: " << error.message() << error.value() << std::endl;
        }
    }
    else {         
        bool preserve;
        std::error_code error = copy_file(src_path, dst_path, preserve = false);
        if (error) {
            std::cerr << "Error: " << error.message() << error.value() << std::endl;
        }
    }
    return 0;
}

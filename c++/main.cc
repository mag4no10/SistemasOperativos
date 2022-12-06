#include <iostream>

#include "copy_move.h"
#include "loose_functions.h"

int main(int argc, char* argv[]) {
    usage(argc, argv);

    std::string mode     (argv[1]);
    std::string src_path (argv[2]);
    std::string dst_path (argv[3]);

    if (mode == "-m") {
        move_file(src_path, dst_path);
    }
    else if (mode == "-a") {
        bool preserve;;
        copy_file(src_path, dst_path, preserve = true);
    }
    else {
        //error
    }
    return 0;
}

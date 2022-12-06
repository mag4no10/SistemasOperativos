#include <iostream>

#include "functions.h"

int main(int argc, char* argv[]) {
    std::string mode = argv[1];
    char* src_path = argv[2];
    char* dst_path = argv[3];
    
    if (mode == "-m") {

    }
    else if (mode == "-a") {
        bool preserve_all = true;
        copy_file(src_path, dst_path, preserve_all);
    }
    else {
        //error
    }
    return 0;
}

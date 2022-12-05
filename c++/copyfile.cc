#include <iostream>
#include <string>
#include <vector>
#include <unistd.h> //read write
#include <fcntl.h> //open 

#include "functions.h"

int main(int argc, char* argv[]) {
    std::string mode = getenv(argv[1]);
    char* src_path = argv[2];
    char* dst_path = argv[3];
    
    /*if (mode == "-m") {

    }
    else if (mode == "-a") {

    }
    else {
        //error
    }
    */
    int src_fd = open(src_path,O_RDONLY);
    if (src_fd < 0) {
        //error
    }
    int dst_fd = open(dst_path, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (dst_fd < 0) {
        //error
    }
    ssize_t nread;
    //const int sz = 16UL * 1024 * 1024;
    //std::vector<uint8_t> buffer(sz);
    char buf[4096];
    while (nread = read(src_fd, buf, sizeof buf), nread > 0) {
        char* out_ptr = buf;
        //for(const auto& i : buffer) {
        //    out_ptr += i;
        //}
        ssize_t nwritten;
        do {
            nwritten = write(dst_fd,out_ptr,nread);
            if (nwritten >= 0) {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            //else if {
            // error
            //}
        } while (nread > 0);
            
        if (nread == 0) {
            //error
            close(src_fd);
            /* Success! */
        }
    }
    return 0;
}

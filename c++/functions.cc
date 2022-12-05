#include <iostream>
#include <vector>
#include <unistd.h>

#include "functions.h"


/*void copy_file(const std::string& src_path, const std::string& dst_path, bool preserve_all=false) {

}

void move_file(const std::string& src_path, const std::string& dst_path) {

}
*/

std::string getenv(const std::string& name) {
    char* value = getenv(name.c_str());
    if (value) {
        return std::string(value);
    }
    else {
        return std::string();
    }
}

/*int read(int fd, std::vector<uint8_t>& buffer) {
    ssize_t bytes_read = read(fd, buffer.data(), buffer.size());
    if (bytes_read < 0) {
        //codigo error
    }
    buffer.resize(bytes_read);
    return 0;
}*/
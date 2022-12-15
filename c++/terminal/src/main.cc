#include <iostream>
#include <vector>
#include <algorithm>

#include "in_out_functions.h"
#include "terminal_functions.h"

int main(int argc, char* argv[]) {
    uint8_t prueba[] = {'H','o','\n','a'};
    pending_input.resize(100);
    int contador{0};
    for (uint8_t i : pending_input) {
        i = prueba[contador];
        contador++;
        if (contador > 4) break;
    }
    std::string linea;
    int fd;
    read_line(fd,linea);
    std::cout << linea << std::endl;
    return 0;
}
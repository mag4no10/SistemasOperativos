#include <iostream>
#include <unistd.h>

#include "in_out_functions.h"
#include "terminal_functions.h"

int main(int argc, char* argv[]) {
    print_prompt(0);
    read_line(STDIN_FILENO,linea);

    return 0;
}
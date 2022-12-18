#include <iostream>
#include <unistd.h>

#include "in_out_functions.h"
#include "terminal_functions.h"


int main(int argc, char* argv[]) {
    while (true) {
        print_prompt(last_command_status);
        read_line(STDIN_FILENO,linea);
        if (!linea.empty()) {
            std::vector<shell::command> list_of_commands = parse_line(linea);
            if (!list_of_commands.empty()) {
                auto [return_value, is_quit_requested] = execute_commands(list_of_commands);
                last_command_status = return_value;
                if (is_quit_requested) {
                    break;
                }
            }
        }
    }
    return 0;
}
#include "in_out_functions.h"
#include "terminal_functions.h"

int main(int argc, char* argv[]) {
    while (true) {
        std::error_code error = print_prompt(last_command_status);
        if (error) {
            last_command_status = 1;
            return last_command_status;
        }
        std::error_code error1 = read_line(STDIN_FILENO,linea);
        if (error1) {
            last_command_status = 1;
            return last_command_status;
        }
        if (linea.empty()) {
            continue;
        }
        std::vector<shell::command> list_of_commands = parse_line(linea);
        if (list_of_commands.empty()) {
            last_command_status = 1;
            return last_command_status;
        }

        auto [return_value, is_quit_requested] = execute_commands(list_of_commands);
        last_command_status = return_value;
        if (is_quit_requested || !isatty(STDIN_FILENO)) {
            return return_value;
        }
    }
    return 0;
}
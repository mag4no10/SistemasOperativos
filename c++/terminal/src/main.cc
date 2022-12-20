#include "in_out_functions.h"
#include "terminal_functions.h"

/*TODO 
*   OUTPUT: COLORS
*   FINISH
**/

int main(int argc, char* argv[]) {
    while (true) {
        std::error_code error = print_prompt(last_command_status);
        if (error) {
            return 1;
        }
        std::error_code error1 = read_line(STDIN_FILENO,linea);
        if (error1) {
            return 1;
        }
        if (linea.empty()) {
            continue;
        }
        std::vector<shell::command> list_of_commands = parse_line(linea);
        if (list_of_commands.empty()) {
            continue;
        }
        auto [return_value, is_quit_requested] = execute_commands(list_of_commands);
        last_command_status = return_value;
        if (is_quit_requested) {
            break;
        }
        if (!isatty(STDIN_FILENO)) {
            break;
        }
    }
    return 0;
}
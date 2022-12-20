#pragma once

#include <iostream>      //Basic input and output
#include <system_error>  //std::error_code
#include <vector>        //std::vector
#include <string>        //std::string
#include <unistd.h>      //getlogin_r gethostname getcwd isatty STDOUT_FILENO STDIN_FILENO
#include <limits.h>      //HOST_NAME_MAX, LOGIN_NAME_MAX, PATH_MAX
#include <sstream>       //istringstream
#include <sys/wait.h>    //wait

namespace shell {
    using command = std::vector<std::string>;
    struct command_result {
        int return_value;
        bool is_quit_requested;
        command_result(int return_value, bool request_quit=false) : 
            return_value{return_value},
            is_quit_requested{request_quit} {}
        static command_result quit(int return_value=0) {
            return command_result{return_value, true};
        }
    };
}

inline std::vector<uint8_t> pending_input(0);
inline std::string linea("");
inline int last_command_status{0};

std::error_code print_prompt(const int&);
std::error_code read_line(int, std::string&);
std::vector<shell::command> parse_line(const std::string&);
shell::command_result execute_commands(const std::vector<shell::command>&);
int execute_program(const std::vector<std::string>&, bool);

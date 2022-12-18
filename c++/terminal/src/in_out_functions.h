#pragma once

#ifdef __unix__
    #define OS_Windows 0
#elif defined(_WIN32) || defined(WIN32)    
    #define OS_Windows 1
#endif 

#include <iostream>
#include <vector>


int echo_command(const std::vector<std::string>&);
int cd_command(const std::vector<std::string>&);
int cp_command(const std::vector<std::string>&);
int mv_command(const std::vector<std::string>&);
int clear_command();
int execute_program(const std::vector<std::string>&, bool);
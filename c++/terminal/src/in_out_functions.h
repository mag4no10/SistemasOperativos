#pragma once

#ifdef __unix__
    #define OS_Windows 0
#elif defined(_WIN32) || defined(WIN32)    
    #define OS_Windows 1
#endif 

#include <iostream>      //Basic input and output
#include <vector>        //std::vector
#include <limits.h>      //LOGIN_NAME_MAX, PATH_MAX
#include <unistd.h>      //get_login_r, chdir, close, chown, unlink
#include <sys/stat.h>    //stat
#include <utime.h>       //utime
#include <fcntl.h>       //open
#include <string>        //std::string


std::error_code echo_command(const std::vector<std::string>&);
std::error_code cd_command(const std::vector<std::string>&);
std::error_code cp_command(const std::vector<std::string>&);
std::error_code mv_command(const std::vector<std::string>&);
std::error_code clear_command();

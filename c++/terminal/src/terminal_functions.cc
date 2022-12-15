#include <iostream>
#include <system_error>
#include <vector>
#include <string>
#include <algorithm>
#include <unistd.h> //getlogin_r gethostname getcwd isatty STDOUT_FILENO STDIN_FILENO
#include <limits.h> //host, login, path max
#include <sstream> //istringstream

#include "terminal_functions.h"

std::error_code print(const std::string& str) {
    uint32_t bytes_written = write(STDOUT_FILENO, str.c_str(), str.size());
    if (bytes_written < 0) {
        //Manejar error
    }
    return std::error_code(0, std::system_category());
}

std::error_code read(int fd, std::vector<uint8_t>& buffer) {
    uint32_t bytes_read = read(fd, buffer.data(), buffer.size());
    if (bytes_read < 0) {
        return std::error_code(EIO, std::system_category());
    }
    buffer.resize(bytes_read);
    return std::error_code(0, std::system_category());
}

void print_prompt(const int& last_command_status) {
    if (!last_command_status && isatty(STDIN_FILENO)) {
        char hostname[HOST_NAME_MAX];
        char username[LOGIN_NAME_MAX];
        char current_path[PATH_MAX];
        getlogin_r(username, LOGIN_NAME_MAX);
        gethostname(hostname,HOST_NAME_MAX);
        getcwd(current_path,PATH_MAX);
        
        std::string prompt = std::string(username) + "@" + std::string(hostname) + ":" + std::string(current_path) + " $> ";
        print(prompt);
    }
    else {
        std::cout << "$< ";
    }
}

std::error_code read_line(int fd, std::string& line) {
    pending_input.resize(16ul * 1024);
    read(fd,pending_input);
    int counter{1};
    while(true) {
        for (const uint8_t i: pending_input) {
            if (i == '\n') {
                line.assign(pending_input.begin(), pending_input.begin()+counter);
                pending_input.erase(pending_input.begin(), pending_input.begin()+counter);
                return std::error_code(0, std::system_category());
            }
            counter++;
        }
        std::vector<uint8_t> buffer(512);
        std::error_code error = read(fd,buffer);
        if (error) {
            return error;
        }
        if (buffer.size() == 0) {
            if (pending_input.size() != 0) {
                std::copy(pending_input.begin(), pending_input.end(), line.begin());
                line = line + "\n";
                pending_input.clear();
                return std::error_code(0,std::system_category());
            }
        }
        else {
            pending_input.insert(pending_input.end(), buffer.begin(), buffer.end());
        }
    }
    return std::error_code(0, std::system_category());
}

std::vector<shell::command> parse_line(const std::string& line) {
    std::istringstream iss(line);
    std::vector<shell::command> result;
    std::vector<std::string> words;
    while (!iss.eof()) {
        std::string word;
        iss >> word;

        if (word.ends_with("&")) {
            std::cout << word;
        }

        words.push_back(word);
    }
    return result;
}
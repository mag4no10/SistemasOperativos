#include "terminal_functions.h"
#include "in_out_functions.h"
#include "loose_functions.h"


std::error_code print_prompt(const int& last_command_status) {
    if (!isatty(STDIN_FILENO)) {
        return std::error_code(0, std::system_category());
    }
    char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];
    char current_path[PATH_MAX];
    int error = getlogin_r(username, LOGIN_NAME_MAX);
    if (error != 0) {
        perror("No se ha podido obtener el nombre de usuario");
        return std::error_code(errno, std::system_category());
    }
    int error1 = gethostname(hostname,HOST_NAME_MAX);
    if (error1 != 0) {
        perror("No se ha podido obtener el nombre de host");
        return std::error_code(errno, std::system_category());
    }
    char* error2 = getcwd(current_path,PATH_MAX);
    if (error2 == NULL) {
        perror("No se ha podido obtener el directorio actual");
        return std::error_code(errno, std::system_category());
    }
        
    if (!last_command_status) {
        std::string prompt = "╭─" + std::string(username) + "@" + std::string(hostname) + ":" + std::string(current_path) + "\n╰─$> ";
        print(prompt);
    }
    else {
        std::string prompt = "╭─" + std::string(username) + "@" + std::string(hostname) + ":" + std::string(current_path) + "\n╰─$< ";
        print(prompt);
    }
    return std::error_code(0, std::system_category());
}

std::error_code read_line(int fd, std::string& line) {
    pending_input.resize(512ul);
    std::error_code error = read(fd,pending_input);
    if (error) {
        return error;
    }
    if (!isatty(STDIN_FILENO)) {
        pending_input.push_back('\n');
    }
    if (pending_input.empty()) {
        line.clear();
        line += '\n';
        return std::error_code(0, std::system_category());
    }
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
        std::vector<uint8_t> buffer(512ul);
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
        if (pending_input.empty()) {
            linea.clear();
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
        if (word.starts_with("#")) {
            break;
        }
        if (word.ends_with(';') || word.ends_with('&') || word.ends_with('|')) {
            std::string aux;
            aux.push_back(word.back());
            word.pop_back();
            words.push_back(word);
            words.push_back(aux);
            result.push_back(words);
            words.clear();
            continue;
        }
        std::string word_former;
        for (char i : word) {
            if (i == ';' || i == '|') {
                words.push_back(word_former);
                std::string aux;
                aux.push_back(i);
                words.push_back(aux);
                result.push_back(words);
                words.clear();
            }
            word_former = word_former + i;
        }
        words.push_back(word);
    }
    result.push_back(words);
    return result;
}

shell::command_result execute_commands(const std::vector<shell::command>& commands) {
    int return_value{0};
    for (std::vector<std::string> i : commands) {
        if (i.back() == ";" || i.back() == "|" || i.back() == "&" || i.back() == "") {
            i.pop_back();
        }
        if (i.front() == "echo") {
            std::error_code error = echo_command(i);
            if (error) {
                return_value = 1;
            }
        }
        else if (i.front() == "cd") {
            std::error_code error1 = cd_command(i);
            if (error1) {
                return_value = 1;
            }
        }
        else if (i.front() == "cp") {
            std::error_code error2 = cp_command(i);
            if (error2) {
                return_value = 1;
            }
        }
        else if (i.front() == "mv") {
            std::error_code error3 = mv_command(i);
            if (error3) {
                return_value = 1;
            }
        }
        else if (i.front() == "exit") {
            return shell::command_result::quit(return_value);
        }
        else if (i.front() == "clear") {
            std::error_code error4 = clear_command();
            if (error4) {
                return_value = 1;
            }
        }
        else if (i.front() == "") {
            return shell::command_result(return_value,false);
        }
        else {
            bool has_to_wait = true;
            if (i.back() == "&") {
                has_to_wait = false;
            }
            int output = execute_program(i,has_to_wait);
            if (output == 1) {
                return_value = 1;
            }
        }
    }
    return shell::command_result{return_value, false};
}

int execute_program(const std::vector<std::string>& args, bool has_wait=true) {
    std::vector<const char*> argv;
    for(uint64_t i = 0; i < args.size(); i++) {
        argv.push_back(args[i].c_str());
    }
    argv.push_back(NULL);
    const char* program_name = argv[0];
    pid_t cpid = fork(); 
    if (cpid == 0) {
        int status_code = execvp(program_name, const_cast<char* const*>(argv.data()));
        if (status_code < 0) {
            std::cout << "terminal: unknown command" << std::endl;
            _exit(1);
        }
        _exit(0);
    }
    else if (cpid > 0) {
        int status;
        if (wait(&status) == -1) {
            perror("wait() failed");
            return 1;
        }
        else {
            int child_returned = WEXITSTATUS(status);
            if (child_returned == 1) {
                return 1;
            }
            return 0;
        }
    }
    else {
        return 1;
    }
    return 0;
}

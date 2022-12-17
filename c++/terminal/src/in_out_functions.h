#pragma once

#include <iostream>
#include <vector>


int echo_command(const std::vector<std::string>&);
int cd_command(const std::vector<std::string>&);
int cp_command(const std::vector<std::string>&);
int mv_command(const std::vector<std::string>&);

int execute_program(const std::vector<std::string>&, bool);
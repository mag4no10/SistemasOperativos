#pragma once

#include <iostream>


std::string getenv(
    const std::string& name
);

std::string dirname(
    std::string& path
);

std::string basename_(
    std::string& path
);

void information(

);

void usage(
    int argc,
    char* argv[]
);

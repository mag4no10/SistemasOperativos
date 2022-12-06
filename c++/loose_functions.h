#pragma once

#include <iostream>


std::string getenv(
    const std::string& name
);

std::string dirname(
    const std::string& path
);

std::string basename(
    const std::string& path
);

void information(

);

void usage(
    int argc,
    char* argv[]
);

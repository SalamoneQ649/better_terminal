#pragma once
#include <iostream>
#include <string>

namespace UI {
    // Kody kolorów ANSI
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[1;31m";
    const std::string GREEN   = "\033[1;32m";
    const std::string YELLOW  = "\033[1;33m";
    const std::string BLUE    = "\033[1;34m";
    const std::string CYAN    = "\033[1;36m";
    const std::string BOLD    = "\033[1m";
    inline void printPrompt(const std::string& currentPath = "bt") {
        std::cout << GREEN << "bt-user@" << CYAN << currentPath << RESET << GREEN << " > " << RESET;
    }
}
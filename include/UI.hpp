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

    inline void printBanner() {
        std::cout << CYAN << R"(
  ____  _____ _____ _____ _____ _____     _____ _____ _____ _____ 
 | __ \| ____|_   _|_   _| ____|  _  |   |_   _| ____|  _  |_   _|
 | __ <|  _|   | |   | | |  _| | |_| |     | | |  _| | |_| | | |  
 |____/|_____| |_|   |_| |_____|_| |_|     |_| |_____|_| |_| |_|  
        )" << RESET << "\n";
        std::cout << GREEN << " [ BetterTerminal v1.0.0 | ESP32 & Modular CLI Environment ]\n" << RESET;
        std::cout << YELLOW << " Wpisz 'help', aby wyswietlic liste komend.\n\n" << RESET;
    }

    inline void printPrompt(const std::string& currentPath = "bt") {
        std::cout << GREEN << "bt-user@" << CYAN << currentPath << RESET << GREEN << " > " << RESET;
    }
}
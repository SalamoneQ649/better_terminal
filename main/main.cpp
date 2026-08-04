#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "../include/UI.hpp"
#include "../include/CommandManager.hpp"

int main() {
    // Włączenie obsługi kolorów ANSI w konsoli Windowsa
    system(""); 

    UI::printBanner();
    CommandManager cmdManager;

    std::string input;
    while (true) {
        UI::printPrompt();

        if (!std::getline(std::cin, input) || input == "exit") {
            std::cout << UI::YELLOW << "Zamykanie BetterTerminal...\n" << UI::RESET;
            break;
        }

        if (input.empty()) continue;

        // Parsowanie tekstu na komendę i argumenty
        std::stringstream ss(input);
        std::string cmdName;
        ss >> cmdName;

        std::vector<std::string> args;
        std::string arg;
        while (ss >> arg) {
            args.push_back(arg);
        }

        // Wykonanie komendy lub błąd
        if (!cmdManager.execute(cmdName, args)) {
            std::cout << UI::RED << "Nieznana komenda: '" << cmdName 
                      << "'. Wpisz 'help', aby zobaczyc liste.\n" << UI::RESET;
        }
    }

    return 0;
}
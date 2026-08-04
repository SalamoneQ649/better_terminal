#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include "UI.hpp"

// Typ funkcji obsługującej komendę
using CommandHandler = std::function<void(const std::vector<std::string>& args)>;

class CommandManager {
private:
    std::map<std::string, CommandHandler> commands;

public:
    CommandManager() {
        registerBuiltins();
    }

    void registerCommand(const std::string& name, CommandHandler handler) {
        commands[name] = handler;
    }

    bool execute(const std::string& name, const std::vector<std::string>& args) {
        if (commands.find(name) != commands.end()) {
            commands[name](args);
            return true;
        }
        return false;
    }

private:
    void registerBuiltins() {
        // Wbudowana komenda: help
        registerCommand("help", [this](const std::vector<std::string>& args) {
            std::cout << UI::YELLOW << "Dostepne komendy:\n" << UI::RESET;
            for (const auto& [name, _] : commands) {
                std::cout << " - " << UI::CYAN << name << UI::RESET << "\n";
            }
        });

        registerCommand("clear","cls", [](const std::vector<std::string>& args) {
            std::cout << "\033[2J\033[1;1H";
        });
        registerCommand("install", [](const std::vector<std::string>& args) {
            if (args.empty()) {
                std::cout << UI::RED << "Brak nazwy pakietu! Uzycie: install <nazwa_pakietu>\n" << UI::RESET;
                return;
            }
            std::cout << UI::CYAN << "[BT] Pobieranie pakietu '" << args[0] << "' z Twojej domeny...\n" << UI::RESET;
        });
        registerCommand
    }
};
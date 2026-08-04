#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include <windows.h>
#include "UI.hpp"

// Typ funkcji obsługującej komendę wewnątrz terminala
using CommandHandler = std::function<void(const std::vector<std::string>& args)>;

// Typ eksportowanej funkcji z pliku DLL (styl C)
typedef void (*PluginExecuteFunc)(int, const char**);

class CommandManager {
private:
    std::map<std::string, CommandHandler> commands;
    std::vector<HMODULE> loadedPlugins; // Przechowuje uchwyty do załadowanych DLL

public:
    CommandManager() {
        registerBuiltins();
    }

    ~CommandManager() {
        // Zwalnianie pamięci po załadowanych DLL przy zamykaniu terminala
        for (HMODULE hModule : loadedPlugins) {
            if (hModule) FreeLibrary(hModule);
        }
    }

    // Dodawanie pojedynczej komendy
    template <typename T>
    void registerCommand(const std::string& name, T handler) {
        commands[name] = CommandHandler(handler);
    }

    // Dodawanie komendy z aliasem (np. clear i cls)
    template <typename T>
    void registerCommand(const std::string& first, const std::string& second, T handler) {
        registerCommand(first, handler);
        registerCommand(second, handler);
    }

    // Dynamiczne ładowanie wtyczki z pliku .dll
    bool loadPlugin(const std::string& commandName, const std::string& dllPath) {
        // 1. Otwarcie pliku DLL w pamięci
        HMODULE hModule = LoadLibraryA(dllPath.c_str());
        if (!hModule) {
            std::cout << UI::RED << "[BT Error] Nie udalo sie zaladowac pliku: " << dllPath << UI::RESET << "\n";
            return false;
        }

        // 2. Pobranie adresu funkcji "execute" z pliku DLL
        PluginExecuteFunc pluginFunc = (PluginExecuteFunc)GetProcAddress(hModule, "execute");
        if (!pluginFunc) {
            std::cout << UI::RED << "[BT Error] Nie znaleziono punktu wejscia 'execute' w " << dllPath << UI::RESET << "\n";
            FreeLibrary(hModule);
            return false;
        }

        // 3. Rejestracja funkcji z DLL i konwersja argumentów na format C (int, const char**)
        registerCommand(commandName, [pluginFunc](const std::vector<std::string>& args) {
    if (!pluginFunc) {
        std::cout << UI::RED << "[BT Error] Funkcja execute jest nieprawidlowa!" << UI::RESET << "\n";
        return;
    }

    std::vector<const char*> c_args;
    for (const auto& arg : args) {
        c_args.push_back(arg.c_str());
    }

    pluginFunc(static_cast<int>(c_args.size()), c_args.data());
});

        loadedPlugins.push_back(hModule);
        return true;
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

        registerCommand("clear", "cls", [](const std::vector<std::string>& args) {
            std::cout << "\033[2J\033[1;1H";
        });

        registerCommand("install", [this](const std::vector<std::string>& args) {
            if (args.empty()) {
                std::cout << UI::RED << "Brak nazwy pakietu! Uzycie: install <nazwa_pakietu>\n" << UI::RESET;
                return;
            }

            std::string pluginName = args[0];
            std::cout << UI::CYAN << "[BT] Pobieranie pakietu '" << pluginName << "' z Twojej domeny...\n" << UI::RESET;

            std::string dllPath = "../plugins/" + pluginName + ".dll";

            // Próba automatycznego załadowania po pobraniu
            if (loadPlugin(pluginName, dllPath)) {
                std::cout << UI::GREEN << "[BT] Pomyslnie zainstalowano i zaladowano komende: " << pluginName << UI::RESET << "\n";
            }
        });

        registerCommand("exit", "quit", [](const std::vector<std::string>& args) {
            std::string choice;
            std::cout << "Are you sure? (Y/N): ";
            std::cin >> choice;
            if (choice == "Y" || choice == "y") {
                std::cout << "Exiting...\n";
                exit(0);
            }
        });
    }
};
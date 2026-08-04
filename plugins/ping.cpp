#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <array>
#include <cstdio>

// extern "C" zapobiega dekorowaniu nazw funkcji przez kompilator C++ (Name Mangling)
// __declspec(dllexport) mówi Windowsowi, że ta funkcja jest widoczna na zewnątrz DLL
extern "C" __declspec(dllexport) void execute(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Uzycie: ping <adres_ip_lub_host>\n";
        return;
    }

    std::string host = args[0];
    std::string command = "ping -n 4 " + host;

    std::cout << "[BT Plugin] Pinging " << host << "...\n";

    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(command.c_str(), "r"), _pclose);
    if (!pipe) {
        std::cout << "Blad podczas uruchamiania procesu ping!\n";
        return;
    }

    std::array<char, 128> buffer;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        std::cout << buffer.data();
    }
}
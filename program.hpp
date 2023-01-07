#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <unordered_map>
#include <string>
#include "action.hpp"

class Program {
    static void AddToCurrentSpace_(Action a);
    static void ExecuteFileExtension_(Action script_name, Action a);
    static void ExecuteActionExtension_(Action script_name, Action a);
public:
    Program(std::string dir_name);
    void LoadDefaultScripts();
    void MainLoop();
    void SetDefaults();
    void LoadUserDefaults();
};

#endif

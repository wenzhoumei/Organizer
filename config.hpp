#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <unordered_map>
#include "action.hpp"

namespace config {
    enum ExitCode { OK = 0 };

    enum Extension { EXTENSION_SCRIPT, SPACE };

    void SetDefaults();

    extern std::unordered_map<Extension, std::string> extensions;
    extern std::string working_directory;

    extern std::unordered_map<Action, std::string> scripts;
    void LoadUserDefaults();
};

#endif

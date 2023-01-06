#include <string>
#include <unordered_map>

#include "config.hpp"

namespace config {

    std::unordered_map<Extension, std::string> extensions;
    std::string working_directory;

    void SetDefaults() {
	extensions[Extension::EXTENSION_SCRIPT] = "ext";
	extensions[Extension::SPACE] = "space";

	working_directory = "/home/wenzhou/Organizer/files";
    }

    void LoadUserDefaults() {
    }
};

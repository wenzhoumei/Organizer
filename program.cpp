#include <filesystem>
#include "config.hpp"
#include "space.hpp"
#include "action.hpp"
#include "program.hpp"

Space Program::LoadScripts() {
    Space scripts;
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
	Action a = Action(entry.path().filename());
	if (a == config::extensions[config::EXTENSION_SCRIPT]) {
	    scripts.push_back(a);
	}
    }

    return scripts;
}

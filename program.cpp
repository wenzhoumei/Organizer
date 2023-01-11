#include <filesystem>
#include <string>
#include "space.hpp"
#include "program.hpp"
#include "action.hpp"
#include <unordered_map>
#include <iostream>
#include <functional>
#include <fstream>

Program::Program(std::string working_directory) {
    std::filesystem::current_path(working_directory);
}


void Program::LoadDefaultScripts() {
    Action::extension_functions[Action::program_actions[Extension::ADD_TO_SPACE]] = Action::AddToCurrentSpace_;

    for (const auto& entry : std::filesystem::directory_iterator(".")) {
	Action action = Action(entry.path().filename());

	// Load extension scripts - {extension}.ext - and save them as functions in map
	std::string extension = action.Extension('.');
	if (extension == "ext") {

	    std::function<void(Action)> script_func = std::bind(Action::ExecuteActionExtension_, action, std::placeholders::_1);
	    Action::extension_functions[action.Name('.')] = script_func;
	}

    }

    std::function<void(Action)> script_func = std::bind(Action::ExecuteFileExtension_, "." + Action::program_actions[Extension::EXTENSION_SCRIPT], std::placeholders::_1);
    Action::extension_functions[""] = script_func;

    for (auto const& [key, val] : Action::extension_functions) {
	Action action = key + "." + Action::program_actions[Extension::EXTENSION_SCRIPT];

	for (const Action& file_extension : action.LoadSpace()) {
	    std::function<void(Action)> script_func = std::bind(Action::ExecuteFileExtension_, action, std::placeholders::_1);
	    Action::extension_functions[file_extension.Name('.')] = script_func;
	}
    }
}

void Program::MainLoop() {
    while (true) {
	std::cout << Action::current_action << " > ";
	Action input;

	getline(std::cin, input);

	if (input == Action("q")) {
	    break;
	}

	input.Run2();
    }
}

void Program::SetDefaults() {
    Action default_startup_action = "program.program";
    default_startup_action.MakeCurrent();

    Action::program_actions[Extension::EXTENSION_SCRIPT] = "ext";
    Action::program_actions[Extension::SPACE] = "space";
    Action::program_actions[Extension::ADD_TO_SPACE] = "add";
    Action::program_actions[Extension::EDIT] = "edit";
    Action::program_actions[Extension::DELETE_FILE] = "delete";
    Action::program_actions[Extension::REMOVE_FROM_SPACE] = "remove";
}


void Program::LoadUserDefaults() {
}

#include <filesystem>
#include <string>
#include "config.hpp"
#include "space.hpp"
#include "program.hpp"
#include "action.hpp"
#include "navigation.hpp"
#include <unordered_map>
#include <iostream>
#include <functional>
#include <fstream>

Program::Program(std::string working_directory) {
    std::filesystem::current_path(working_directory);
}


void Program::LoadDefaultScripts() {
    Action::extension_functions["add"] = AddToCurrentSpace_;
    std::function<void(Action)> script_func = std::bind(ExecuteFileExtension_, ".ext", std::placeholders::_1);
    Action::extension_functions[""] = AddToCurrentSpace_;

    for (const auto& entry : std::filesystem::directory_iterator(".")) {
	Action action = Action(entry.path().filename());

	// Load extension scripts - {extension}.ext - and save them as functions in map
	std::string extension = action.Extension('.');
	if (extension == "ext") {
	    std::cout << action.Name('.') << std::endl;

	    std::function<void(Action)> script_func = std::bind(ExecuteActionExtension_, action, std::placeholders::_1);
	    Action::extension_functions[action.Name('.')] = script_func;
	}

	Space default_file_extensions = action.LoadSpace();
	for (const Action& file_extension : default_file_extensions) {
	    std::cout << "haha" << std::endl;
	    std::function<void(Action)> script_func = std::bind(ExecuteFileExtension_, action, std::placeholders::_1);
	    Action::extension_functions[file_extension.Name('.')] = script_func;
	}
    }
}

void Program::MainLoop() {
    while (true) {
	std::cout << "> ";
	Action input;

	getline(std::cin, input);

	if (input == Action("q")) {
	    break;
	}

	input.Run2();
    }
}
void Program::SetDefaults() {
    config::default_action_extensions[config::Extension::EXTENSION_SCRIPT] = "ext";
    config::default_action_extensions[config::Extension::SPACE] = "space";
}


void Program::LoadUserDefaults() {
}

void Program::AddToCurrentSpace_(Action a) {
    if (navigation::current_space.ContainsAction(a)) {
	std::string test;
	std::cerr << "Action exists in current space: " << a << std::endl;
	return;
    }

    navigation::current_space.push_back(a);

    std::ofstream outfile;
    outfile.open(navigation::current_action.GetSpaceName(), std::ios_base::app);
    outfile << a << std::endl;
    outfile.close();

    return;
}

void Program::ExecuteFileExtension_(Action script_name, Action a) {
    std::string command = "./" + script_name + " " + "\"" + a + "\"";

    int result = WEXITSTATUS(std::system(command.c_str()));
    if (result != config::ExitCode::OK) {
	std::cout << "Error: script returned non-zero exit code: " << result << std::endl;
    }
}

void Program::ExecuteActionExtension_(Action script_name, Action a) {
    std::string command = "./" + script_name + " " + "\"" + a.GetScriptParameter() + "\"";

    int result = WEXITSTATUS(std::system(command.c_str()));
    if (result != config::ExitCode::OK) {
	std::cout << "Error: script returned non-zero exit code: " << result << std::endl;
    }
}

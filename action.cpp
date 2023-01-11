#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "action.hpp"
#include "space.hpp"

Action Action::current_action;

std::unordered_map<std::string, std::function<void(Action)>> Action::extension_functions;
std::unordered_map<enum Extension, Action> Action::program_actions;

std::string Action::Extension(char delimiter) const
{
    size_t delimiterPos = this->find_last_of(delimiter);
    if (delimiterPos == std::string::npos)
	return "";
    else
	return this->substr(delimiterPos + 1);
}

std::string Action::Name(char delimiter) const
{
    size_t delimiterPos = this->find_last_of(delimiter);
    if (delimiterPos == std::string::npos)
	return *this;
    else
	return this->substr(0, delimiterPos);
}

bool Action::IsFile() const {
    std::ifstream file(*this);
    return file.good();
}

// if file doesn't exist set arg to action name else set it to file_str
std::string Action::GetScriptParameter() const {
    std::ifstream file(*this);
    if (file.good()) {
	// read the contents of the file into a string
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	// close the file
	file.close();

	// run the script with 'arg' as the argument
	return contents;
    } else {
	return this->Name('.');
    }
}

/* Reads a file and returns a vector of strings containing the lines of the file.
 *
 * Returns a vector of strings on success, an empty vector on error.
 */
Space Action::LoadSpace() const {
    std::ifstream file(*this + ".space");
    Space lines;

    if (file.good()) {
	Action line;
	while (std::getline(file, line))
	    lines.push_back(line);
    }

    return lines;
}

Action Action::GetSpaceName() const {
    return *this + "." + program_actions[Extension::SPACE];
}

Action Action::GetScriptName() const {
    return this->Extension('.') + "." + program_actions[Extension::EXTENSION_SCRIPT];
}

void Action::Run() const {
    Action script_name = this->GetScriptName();

    std::string param;

    if (!script_name.IsFile()) {
	// if script doesn't exist run default function .ext
	param = *this;
	script_name = "." + program_actions[Extension::EXTENSION_SCRIPT];
    } else {
	param = this->GetScriptParameter();
    }

    std::string command = "./" + script_name + " " + "\"" + param + "\"";
    std::cout << command << std::endl;

    // run command and store exit status code
    int result = WEXITSTATUS(std::system(command.c_str()));
    if (result != ExitCode::OK) {
	std::cout << "Error: script returned non-zero exit code: " << result << std::endl;
    }
}

void Action::MakeCurrent() const {
    current_action = *this;
    Space::current_space = this->LoadSpace();
}

void Action::Run2() const {
    std::string extension = this->Extension('.');
    if (!extension_functions.contains(extension)) {
	// If extension corresponds to a newly created file
	Action script_filename = extension + "." + program_actions[Extension::EXTENSION_SCRIPT];
	if (script_filename.IsFile()) {
	    std::function<void(Action)> script_func = std::bind(ExecuteActionExtension_, script_filename, std::placeholders::_1);
	    extension_functions[script_filename.Name('.')] = script_func;
	} else {
	    extension = "";
	}
    }

    extension_functions[extension](*this);
    MakeCurrent();
}

void Action::ExecuteFileExtension_(Action script_name, Action a) {
    std::string command = "./" + script_name + " " + "\"" + a + "\"";
    std::cout << command << std::endl;

    int result = WEXITSTATUS(std::system(command.c_str()));
    if (result != ExitCode::OK) {
	std::cout << "Error: script returned non-zero exit code: " << result << std::endl;
    }
}

void Action::ExecuteActionExtension_(Action script_name, Action a) {
    std::string command = "./" + script_name + " " + "\"" + a.GetScriptParameter() + "\"";
    std::cout << command << std::endl;

    int result = WEXITSTATUS(std::system(command.c_str()));
    if (result != ExitCode::OK) {
	std::cout << "Error: script returned non-zero exit code: " << result << std::endl;
    }
}

void Action::AddToCurrentSpace_(Action a) {
    Action action = a.Name('.');
    if (Space::current_space.ContainsAction(action)) {
	std::cerr << "Action exists in current space: " << a << std::endl;
	return;
    }

    if (current_action.Extension('.') == program_actions[Extension::EXTENSION_SCRIPT]) {
	    std::function<void(Action)> script_func = std::bind(ExecuteFileExtension_, current_action, std::placeholders::_1);
	    extension_functions[action] = script_func;
    }

    Space::current_space.push_back(a);

    std::ofstream outfile;
    outfile.open(current_action.GetSpaceName(), std::ios_base::app);
    outfile << action << std::endl;
    outfile.close();

    return;
}

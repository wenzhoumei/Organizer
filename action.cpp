#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "action.hpp"
#include "config.hpp"
#include "space.hpp"

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
Space Action::LoadSpace()
{
    std::ifstream file(*this + config::extensions[config::Extension::SPACE]);
    Space lines;

    if (file.good()) {
	Action line;
	while (std::getline(file, line))
	    lines.push_back(line);
    }

    return lines;
}

Action Action::GetScriptName() const {
    return this->Extension('.') + "." + config::extensions[config::EXTENSION_SCRIPT];
}

void Action::Run() const {
    Action script_name = this->GetScriptName();

    std::string param;

    if (!script_name.IsFile()) {
	// if script doesn't exist run default function .ext
	param = *this;
	script_name = "." + config::extensions[config::EXTENSION_SCRIPT];
    } else {
	param = this->GetScriptParameter();
    }

    std::string command = "./" + script_name + " " + "\"" + param + "\"";
    std::cout << command << std::endl;

    // run command and store exit status code
    int result = WEXITSTATUS(std::system(command.c_str()));
    if (result != config::ExitCode::OK) {
	std::cerr << "Error: script returned non-zero exit code: " << result << std::endl;
    }
}

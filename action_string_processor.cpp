#include "action_string_processor.hpp"

std::string ActionStringProcessor::GetPureExtension(const std::string& action_string) const
{
    size_t delimiterPos = action_string.find_last_of('.');
    if (delimiterPos == std::string::npos) {
	return program_extensions_[Extension::DEFAULT];
    } else {
	return action_string.substr(delimiterPos + 1);
    }
}

std::string ActionStringProcessor::GetPureName(const std::string& action_string) const
{
    size_t delimiterPos = action_string.find_last_of('.');
    if (delimiterPos == std::string::npos) {
	return action_string;
    } else {
	return action_string.substr(0, delimiterPos);
    }
}

std::string ActionStringProcessor::GetExtension(const std::string& action_string) const
{
    std::string pure_extension = GetPureExtension(action_string);
    return extension_preloads_.contains(pure_extension) ? pure_extension: program_extensions_[Extension::DEFAULT];
}

std::string ActionStringProcessor::GetName(const std::string& action_string) const
{
    size_t delimiterPos = action_string.find_last_of('.');
    if (delimiterPos == std::string::npos) {
	return action_string; 
    } else { 
	std::string extension = action_string.substr(delimiterPos + 1);
	std::string name = action_string.substr(0, delimiterPos);
	return extension_preloads_.contains(extension) ? name: action_string;
    }
}
void ActionStringProcessor::ExecuteFileExtension(std::string script_name, std::string action_string) const {
    std::string command = "./" + script_name + " " + "\"" + action_string + "\"";
    std::cout << command << std::endl;

    int result = WEXITSTATUS(std::system(command.c_str()));
    if (result != ExitCode::OK) {
	std::cout << "Error: script returned non-zero exit code: " << result << std::endl;
    }
}

std::string ActionStringProcessor::GetParameter(std::string action_string) const {
    std::ifstream file(action_string);
    if (file.good()) {
	// read the contents of the file into a string
	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	// run the script with 'arg' as the argument
	return contents;
    } else {
	return GetPureName(action_string);
    }
}

void ActionStringProcessor::ExecuteActionExtension(std::string script_name, std::string action_string) const {
    std::string command = "./" + script_name + " " + "\"" + GetParameter(action_string) + "\"";
    std::cout << command << std::endl;

    int result = WEXITSTATUS(std::system(command.c_str()));
    if (result != ExitCode::OK) {
	std::cout << "Error: script returned non-zero exit code: " << result << std::endl;
    }
}

bool ActionStringProcessor::IsValidColour(const QString& color) const {
    QRegularExpression colorRegex("#[0-9A-F]{6}");
    return colorRegex.match(color).hasMatch();
}

void ActionStringProcessor::AssignColour(const std::string& extension_name, QString& colour_location) const {
	    std::ifstream file_read(extension_name + '.' + program_extensions_[Extension::COLOUR], std::ios::in);
	    QString colour = QString::fromStdString(std::string((std::istreambuf_iterator<char>(file_read)), std::istreambuf_iterator<char>()));
	    file_read.close();

	    if (!IsValidColour(colour)) {
		std::cerr << "Invalid colour: " << colour.toStdString() << std::endl;
		std::ofstream file_write(extension_name + '.' + program_extensions_[Extension::COLOUR], std::ios::trunc);
		colour_location = GenerateRandomColour_();
		file_write << colour_location.toStdString() << std::endl;
		file_write.flush();
		file_write.close();
	    } else {
		colour_location = colour;
	    }
}

void ActionStringProcessor::PreloadExtensions() const {
    for (const auto& entry : std::filesystem::directory_iterator(directory_path_)) {
	std::string script_action_string = entry.path().filename();

	// Load extension scripts - {extension}.ext - and save them as functions in map
	if (GetPureExtension(script_action_string) == program_extensions_[Extension::EXTENSION_SCRIPT]) {
	    std::string extension_name = GetPureName(script_action_string);
	    struct PreloadedExtension preloaded;
	    preloaded.run_function  = [this, script_action_string](std::string action_string) { this->ExecuteActionExtension(script_action_string, action_string); };
	    AssignColour(extension_name, preloaded.colour);

	    extension_preloads_[extension_name] = preloaded;
	}
    }

    for (auto const& [key, val] : extension_preloads_) {
	std::string script_action_string = key + "." + program_extensions_[Extension::EXTENSION_SCRIPT];

	for (const std::string& file_extension : LoadSpace(script_action_string)) {
	    std::string extension_name = GetPureName(file_extension);

	    struct PreloadedExtension preloaded;
	    preloaded.run_function  = [this, script_action_string](std::string action_string) { this->ExecuteFileExtension(script_action_string, action_string); };

	    AssignColour(extension_name, preloaded.colour);

	    extension_preloads_[extension_name] = preloaded;
	}
    }
}

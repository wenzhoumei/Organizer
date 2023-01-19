#ifndef ACTION_STRING_HPP
#define ACTION_STRING_HPP

#include <string>
#include <QString>
#include <unordered_map>

#include <random>
#include <sstream>
#include <string>
#include <iomanip>

#include <filesystem>
#include <iterator>

#include <functional>

#include <fstream>

#include <iostream>

#include <QRegularExpression>


enum Extension { EXTENSION_SCRIPT, COLOUR, DEFAULT, SPACE };
enum ExitCode { OK = 0 };

class ProcessedString {
private:
    std::string extension;
    std::string parameter;

    std::string display_string;
    bool global;

public:
    void Execute() {
    }
};

class Program {
private:
    struct PreloadedExtension {
	QString colour;
	std::function<void(std::string)> run_function;
    };

    std::unordered_map<std::string, PreloadedExtension> extension_preloads_;

    std::string directory_path_;

    std::string action_extension_dir_name_;
    std::string filename_extension_dir_name_;
    std::string colours_extension_dir_name_;
    std::string globals_extension_dir_name_;

    std::string here_file_name_;
    std::string whitelist_file_name_;

private:
    QString GenerateRandomColour_() const;

private:
    void ExecuteFileExtension(std::string script_name, std::string action_string) const;
    void ExecuteActionExtension(std::string script_name, std::string action_string) const;

public:
    Program(std::string directory_path, std::string action_extension_dir_name, std::string filename_extension_dir_name): directory_path_(directory_path) {
    }

public:
    void PreloadExtensions() const; // Find all defined extensions and preload them into a map
    ProcessedString ProcessString(const std::string& action_string) const;
};

/*
class ActionStringProcessor {
private:
    inline static std::unordered_map<enum Extension, std::string> program_extensions_ = { { Extension::EXTENSION_SCRIPT, "ext" }, { Extension::COLOUR, "colour" }, { Extension::DEFAULT, "" }, { Extension::SPACE, "space" } };


    inline static std::unordered_map<std::string, PreloadedExtension> extension_preloads_;

    std::string directory_path_;

    std::string action_extension_dir_name_;
    std::string filename_extension_dir_name_;
    std::string colours_extension_dir_name_;
    std::string globals_extension_dir_name_;

    std::string here_file_name_;
    std::string whitelist_file_name_;

    QString GenerateRandomColour_() const {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);

	std::stringstream ss;
	ss << "#";
	for (int i = 0; i < 3; i++) {
	    int num = dis(gen);
	    ss << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << num;
	}

	return QString::fromStdString(ss.str());
    }

    bool IsValidColour(const QString& color) const;
public:
    ActionStringProcessor(std::string directory_path, std::string action_extension_dir_name, std::string filename_extension_dir_name): directory_path_(directory_path) {
    }

    QString GetColour(const std::string& action_string) const
    {
	return extension_preloads_[GetExtension(action_string)].colour;
    }

    std::function<void(std::string)> GetScript(const std::string& action_string) const
    {
	return extension_preloads_[GetExtension(action_string)].run_function;
    }

    ProcessedString ProcessString(const std::string& action_string) const;
    std::string GetName(const std::string& action_string) const;
    std::string GetExtension(const std::string& action_string) const;
    std::string GetPureName(const std::string& action_string) const;
    std::string GetPureExtension(const std::string& action_string) const;
    std::string GetParameter(std::string action_string) const;

    bool FileExists(const std::string& action_string) const {
	std::ifstream file(action_string);
	return file.good();
    }

    std::vector<std::string> LoadSpace(std::string action_string) const {
	std::ifstream file(action_string + '.' + program_extensions_[Extension::SPACE]);
	std::vector<std::string> lines;

	if (file.good()) {
	    std::string line;
	    while (std::getline(file, line))
		lines.push_back(line);
	}

	return lines;
    }

    void AssignColour(const std::string& extension_name, QString& colour_location) const;
    void PreloadExtensions() const; // Find all defined extensions and preload them into a map
    void ExecuteFileExtension(std::string script_name, std::string action_string) const;
    void ExecuteActionExtension(std::string script_name, std::string action_string) const;

    void PreloadExtensions2() const; // Find all defined extensions and preload them into a map
};
*/

#endif

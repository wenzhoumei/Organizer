#pragma once
#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <filesystem>
#include <functional>
#include <fstream>
#include <random>
#include <regex>

#include <QString>

class ProcessedExtension {
    friend class Program;
    friend class ProcessedString;
private:
    static ProcessedExtension default_extension_;
    static ProcessedExtension reserved_extension_;
    static inline std::unordered_map<std::string, ProcessedExtension> processed_extensions_;

public:
    static void Print() {
	for (auto const& [key, val]: processed_extensions_) {
	    std::cout << "extension: " << key << std::endl;
	    std::cout << "colour: " << val.colour.toStdString() << std::endl;
	}
    }

    static bool SetDefaultExtension(const std::string& action_extension) {
	if (ProcessedExtension::Contains(action_extension)) {
	    return false;
	} else {
	    default_extension_ = processed_extensions_[action_extension];
	    return true;
	}
    }

    static bool SetReservedExtension(const std::string& action_extension) {
	if (ProcessedExtension::Contains(action_extension)) {
	    return false;
	} else {
	    reserved_extension_ = processed_extensions_[action_extension];
	    return true;
	}
    }

    static ProcessedExtension& Get(const std::string& extension_str) {
	return processed_extensions_[extension_str];
    }

    static ProcessedExtension& Count(const std::string& extension_str) {
	return processed_extensions_[extension_str];
    }

    static bool Contains(const std::string& extension_str) {
	return processed_extensions_.contains(extension_str);
    }

private:

    static std::vector<std::string> ReadListFile_(const std::string& filename) {
	std::vector<std::string> ret;
	std::ifstream file(filename);
	if (!file.good()) {
	    std::cerr << "File not found: " << filename << std::endl;
	    return ret;
	}

	std::copy(std::istream_iterator<std::string>(file),
		  std::istream_iterator<std::string>(),
		  std::back_inserter(ret));
	file.close();
	return ret;
    }

    static QString AssignColour_(const std::string& extension_name, const std::filesystem::path& colour_dir_path) {
	auto IsValidColour = [](const std::string& colour) {
	    std::regex colour_regex("^#[0-9A-F]{6}$");
	    return std::regex_match(colour, colour_regex);
	};

	auto GenerateRandomColour = []() {
	    std::random_device rd;
	    std::mt19937 gen(rd());
	    std::uniform_int_distribution<> dis(0, 255);

	    std::stringstream ss;
	    ss << "#";
	    for (int i = 0; i < 3; i++) {
		int num = dis(gen);
		ss << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << num;
	    }

	    return ss.str();
	};

	std::filesystem::path extension_name_path(colour_dir_path/extension_name);

	std::ifstream file_read(extension_name_path, std::ios::in);
	std::string read_colour = std::string((std::istreambuf_iterator<char>(file_read)), std::istreambuf_iterator<char>());
	file_read.close();

	if (!IsValidColour(read_colour)) {
	    std::ofstream file_write(extension_name_path, std::ios::trunc);
	    std::string new_colour = GenerateRandomColour();
	    file_write << new_colour;
	    file_write.flush();
	    file_write.close();

	    if (read_colour != "\n") {
		std::cerr << "Invalid colour: " << read_colour << std::endl;
	    }

	    std::cerr << "Assigning colour: " << new_colour << std::endl;

	    return QString::fromStdString(new_colour);
	} else {
	    return QString::fromStdString(read_colour);
	}
    }

public:
    std::function<int(std::string)> func;
    QString colour;
    ProcessedExtension(const std::function<int(std::string)> func, QString colour)
	:func(func), colour(colour)
    {
    }

    ProcessedExtension()
    {
    }

public:
    static void LoadExtensions(std::filesystem::path action_dir_path, std::filesystem::path filename_dir_path, std::filesystem::path colour_dir_path) {
	for (const auto& entry : std::filesystem::directory_iterator(action_dir_path)) {
	    std::string action_extension = entry.path().filename();

	    std::function<int(std::string)> func = [action_extension](std::string action_string) { return ExecuteActionExtension(action_extension, action_string); };

	    QString col = AssignColour_(action_extension, colour_dir_path);
	    processed_extensions_[action_extension] = ProcessedExtension(func, col);
	}

	for (auto const& [key, val] : processed_extensions_) {
	    std::filesystem::path action_extension = key;
	    for (const std::string& file_extension : ReadListFile_(filename_dir_path/action_extension)) {
		std::function<int(std::string)> func = [action_extension](std::string action_string) { return ExecuteFileExtension(action_extension, action_string); };
		QString col = AssignColour_(action_extension, colour_dir_path);

		processed_extensions_[file_extension] = ProcessedExtension(func, col);
	    }
	}
    }

private:
    static int ExecuteFileExtension(const std::string& script_name, const std::string& action_string) {
	std::string command = "./" + script_name + " " + "\"" + action_string + "\"";
	std::cout << command << std::endl;

	return WEXITSTATUS(std::system(command.c_str()));
    }

    static int ExecuteActionExtension(const std::string& script_name, const std::string& action_string) {
	std::string command = "./" + script_name + " " + "\"" + action_string + "\"";
	//std::string command = "./" + script_name + " " + "\"" + GetParameter(action_string) + "\"";
	std::cout << command << std::endl;

	return WEXITSTATUS(std::system(command.c_str()));
	return -1;
    }
};

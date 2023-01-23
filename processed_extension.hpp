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
    friend class ExtensionList;

private:
    static inline std::filesystem::path colour_dir_path;

    void AssignColour_(const std::string& extension_name) {
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

	    colour = QString::fromStdString(new_colour);
	} else {
	    colour = QString::fromStdString(read_colour);
	}
    }

public:
    enum ExtensionType { FILE_EXTENSION, ACTION_EXTENSION, NONE_EXTENSION } extension_type;
    QString colour;
    std::filesystem::path script_path;
    ProcessedExtension(std::string extension_name, ExtensionType extension_type)
	:extension_type(extension_type)
    {
	AssignColour_(extension_name);
    }

private:
    int ExecuteCommand(const std::string& script_name, const std::string& data) {
	std::string command = "./" + script_name + " " + "\"" + data + "\"";
	std::cout << command << std::endl;

	return WEXITSTATUS(std::system(command.c_str()));
    }
};

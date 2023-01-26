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

#include "extension.hpp"
#include "my_filesystem.hpp"
#include "data_retriever.hpp"

#include <QString>

class Extension {
    static inline Extension* processed_default_action_extension_ = nullptr;
    static inline std::string reserved_file_extension;

    static inline std::unordered_map<std::string, Extension> processed_action_extensions_;
    static inline std::unordered_map<std::string, Extension> processed_data_extensions_;



    static QString AssignColour_(const std::string& extension_name) {
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

	std::filesystem::path extension_name_path(MyFileSystem::GetColoursDirectoryPath()/extension_name);

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

private:
    const static inline std::string delimiters_ = ".|@";
public:
    enum Delimiter { DATA, ACTION, PROGRAM };
    static char GetDelimiter(enum Delimiter delimiter) {
	return delimiters_[delimiter];
    }

    static std::string GetDelimiters() {
	return delimiters_;
    }

    static void Print() {
	for (auto const& [key, val]: processed_action_extensions_) {
	    std::cout << "extension: " << key << std::endl;
	    std::cout << "colour: " << val.colour.toStdString() << std::endl;
	}

	for (auto const& [key, val]: processed_data_extensions_) {
	    std::cout << "extension: " << key << std::endl;
	    std::cout << "colour: " << val.colour.toStdString() << std::endl;
	}
    }

    static Extension* GetProcessedDefaultExtension() {
	return processed_default_action_extension_;
    }

public:
    static std::string GetReservedExtensionName() {
	return reserved_file_extension;
    }

    static bool ContainsProcessedActionExtension(const std::string& extension_str) {
	return processed_action_extensions_.contains(extension_str);
    }

    static Extension* GetProcessedActionExtension(const std::string& extension_str) {
	return &processed_action_extensions_.at(extension_str);
    }

    static bool ContainsProcessedDataExtension(const std::string& extension_str) {
	return processed_data_extensions_.contains(extension_str);
    }

    static Extension* GetProcessedDataExtension(const std::string& extension_str) {
	return &processed_data_extensions_.at(extension_str);
    }

    static void SetReservedExtension(const std::string& extension_name) {
	reserved_file_extension = extension_name;
	processed_data_extensions_.emplace(extension_name, Extension(extension_name, "No colour"));
    }

    static void LoadActionExtensions() {
	for (const std::filesystem::path& entry : std::filesystem::directory_iterator(MyFileSystem::GetActionExtensionsDirectoryPath())) {
	    std::string action_extension_name = entry.filename();
	    if (ContainsProcessedActionExtension(action_extension_name)) { std::cerr << "Conflicting extension name: " <<  action_extension_name << std::endl; }
	    processed_action_extensions_.emplace(action_extension_name, Extension(action_extension_name, AssignColour_(action_extension_name)));
	}
    }

    static void LoadDataExtensions() {
	for (auto const& [key, val] : processed_action_extensions_) {
	    std::filesystem::path action_extension_name = key;
	    for (const std::string& data_extension_name : DataRetriever::ReadActionDataExtensionsListFile(action_extension_name)) {
		if (ContainsProcessedDataExtension(data_extension_name)) { std::cerr << "Conflicting extension name: " <<  data_extension_name << std::endl; }
		processed_data_extensions_.emplace(data_extension_name, Extension(action_extension_name, AssignColour_(action_extension_name)));
	    }
	}
    }

    static bool SetDefaultActionExtension(const std::string& extension_name) {
	if (ContainsProcessedActionExtension(extension_name)) {
	    processed_default_action_extension_ = &processed_action_extensions_.at(extension_name);
	    return true;
	} else {
	    return false;
	}
    }

public:
    std::filesystem::path extension_name;
    QString colour;

    Extension(const std::string& extension_name, const QString& colour)
	:extension_name(extension_name), colour(colour)
    {}
};

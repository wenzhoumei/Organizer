#pragma once

#include <filesystem>
#include <iostream>
#include <unordered_map>
#include "processed_extension.hpp"

class ExtensionList {
    friend class Program;
    friend class ProcessedString;

    ProcessedExtension* default_extension_ = nullptr;
    ProcessedExtension* reserved_extension_ = nullptr;
    std::unordered_map<std::string, ProcessedExtension> processed_extensions_;

public:
    void Print() {
	for (auto const& [key, val]: processed_extensions_) {
	    std::cout << "extension: " << key << std::endl;
	    std::cout << "colour: " << val.colour.toStdString() << std::endl;
	}
    }

    bool SetDefaultExtension(const std::string& extension_name) {
	if (Contains(extension_name)) {
	    default_extension_ = &processed_extensions_.at(extension_name);
	    return true;
	} else {
	    return false;
	}
    }

    void SetReservedExtension(const std::string& extension_name) {
	processed_extensions_.emplace(extension_name, ProcessedExtension(extension_name, ProcessedExtension::NONE_EXTENSION));
    }

    ProcessedExtension* Get(const std::string& extension_str) {
	if (Contains(extension_str)) {
	    return &processed_extensions_.at(extension_str);
	} else {
	    return nullptr;
	}
    }

    bool Contains(const std::string& extension_str) {
	return processed_extensions_.contains(extension_str);
    }

    std::vector<std::string> ReadListFile_(const std::string& filename) {
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

    ExtensionList(std::filesystem::path action_dir_path, std::filesystem::path filename_dir_path, std::filesystem::path colour_dir_path, const std::string reserved_extension, const std::string default_extension)
    {
	ProcessedExtension::colour_dir_path = colour_dir_path;

	SetReservedExtension(reserved_extension);

	for (const auto& entry : std::filesystem::directory_iterator(action_dir_path)) {
	    std::string action_extension_name = entry.path().filename();
	    if (Contains(action_extension_name)) { std::cerr << "Conflicting extension name: " <<  action_extension_name << std::endl; }
	    processed_extensions_.emplace(action_extension_name, ProcessedExtension(action_extension_name, ProcessedExtension::ACTION_EXTENSION));
	}

	for (auto const& [key, val] : processed_extensions_) {
	    std::filesystem::path action_extension = key;
	    for (const std::string& file_extension_name : ReadListFile_(filename_dir_path/action_extension)) {
		if (Contains(file_extension_name)) { std::cerr << "Conflicting extension name: " <<  file_extension_name << std::endl; }
		processed_extensions_.emplace(file_extension_name, ProcessedExtension(file_extension_name, ProcessedExtension::FILE_EXTENSION));
	    }
	}

	if (!SetDefaultExtension(default_extension)) { 
	    std::cerr << "Failed to set default extension: " << default_extension << std::endl;
	    exit(-1);
	}
    }
};

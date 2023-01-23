#pragma once
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>

#include <QString>
#include "processed_extension.hpp"
#include "extension_list.hpp"

class ProcessedString {
private:
    static inline std::filesystem::path current_directory_path_;
    static inline std::filesystem::path global_directory_path_;
    //static cache dict
    
    static inline ExtensionList* extension_list = nullptr;

public:
    static void SetExtensionList(ExtensionList& extension_list) {
	ProcessedString::extension_list = &extension_list;
    }

    static void SetGlobalDirectory(std::filesystem::path global_directory_path) {
	global_directory_path_ = global_directory_path;
    }

    static void SetCurrentDirectory(std::filesystem::path current_directory_path) {
	current_directory_path_ = current_directory_path;
    }

private:
    std::string name_;
    std::string data_;
    ProcessedExtension* extension_ = nullptr;
    std::string display_string_;
    QString colour;

    enum { GLOBAL_FILE, LOCAL_FILE, LOCAL_STRING, DEFAULT, NAME, UNKNOWN } datatype_ = UNKNOWN;

    static constexpr int MAX_STR_LENGTH = 1000;
public:
    ProcessedString(const std::string& action_string) {
	if (action_string.size() > MAX_STR_LENGTH) {
	    std::cerr << "String too big" << std::endl;
	    return;
	}

	// Assign everything before first comma to display_string
	// Assign everything after first comma to data
	std::string extension_str;

	size_t first_comma_pos = action_string.size();
	size_t last_del_pos = MAX_STR_LENGTH + 1;

	for (size_t i = 0; i < action_string.size(); i++) {
	    if (action_string[i] == ',') {
		first_comma_pos = i;
		break;
	    } else if (action_string[i] == '.') {
		last_del_pos = i;
	    } else if (action_string[i] == '@') {
		last_del_pos = i;
	    } else if (action_string[i] == ':') {
		last_del_pos = i;
	    }
	}

	for (size_t i = 0; i < first_comma_pos; i++) { display_string_ += action_string[i]; }

	// Assign data if comma exists
	if (first_comma_pos != action_string.size()) {
	    for (size_t i = first_comma_pos + 1; i < action_string.size(); i++) { data_ += action_string[i]; }
	    datatype_ = LOCAL_STRING;
	}

	if (last_del_pos != MAX_STR_LENGTH + 1) {
	    // If dot exists assign name and extension
	    for (size_t i = 0; i < last_del_pos; i++) { name_ += display_string_[i]; }
	    for (size_t i = last_del_pos + 1; i < display_string_.size(); i++) { extension_str += display_string_[i]; }
	} else {
	    // Else assign default extension string
	    extension_ = extension_list->default_extension_;
	    data_ = action_string;
	    datatype_ = DEFAULT;
	}


	if (extension_list->Contains(extension_str)) {
	    extension_ = extension_list->Get(extension_str);
	} else {
	    extension_ = extension_list->default_extension_;
	    data_ = action_string;
	    datatype_ = DEFAULT;
	}

	if (datatype_ == UNKNOWN) {
	    // Check if it is a file
	    std::ifstream file(display_string_);
	    if (file.good()) {
		// read the contents of the file into a string
		std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		data_ = contents;
		datatype_ = LOCAL_FILE;

		// Set contents of file to data
		data_ = contents;
	    } else {
		for (size_t i = 0; i < last_del_pos; i++) { data_ += action_string[i]; }
		datatype_ = NAME;
	    }
	}

	std::cout << "action_string: " << action_string << std::endl;
	std::cout << "display_string: " << display_string_ << std::endl;
	std::cout << "data: " << data_ << std::endl;
	std::cout << "extension: " << extension_str << std::endl;
    }

    int Execute() {
	return -1;
    }

    QString Colour() {
	return extension_->colour;
    }
};

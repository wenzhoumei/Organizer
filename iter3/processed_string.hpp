#pragma once
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>

#include <QString>
#include "processed_extension.hpp"

class ProcessedString {
private:
    static std::filesystem::path current_directory_path_;
    static std::filesystem::path global_directory_path_;
    //static cache dict

public:
    static void SetGlobalDirectory(std::filesystem::path global_directory_path) {
	global_directory_path_ = global_directory_path;
    }
    static void SetCurrentDirectory(std::filesystem::path current_directory_path) {
	current_directory_path_ = current_directory_path;
    }

private:
    bool data_set_ = false;

    std::string name_;
    std::string data_;
    ProcessedExtension* extension_;
    std::string display_string_;
    QString colour;

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

	size_t first_comma_pos = MAX_STR_LENGTH + 1;
	size_t last_dot_pos = MAX_STR_LENGTH + 1;

	for (size_t i = 0; i < action_string.size(); i++) {
	    if (action_string[i] == ',') {
		first_comma_pos = i;
		break;
	    } else if (action_string[i] == '.') {
		last_dot_pos = i;
	    }
	}

	// Assign display string and data if comma exists
	if (first_comma_pos != MAX_STR_LENGTH + 1) {
	    for (size_t i = 0; i < first_comma_pos; i++) { display_string_ += action_string[i]; }
	    for (size_t i = first_comma_pos + 1; i < action_string.size(); i++) { data_ += action_string[i]; }
	    data_set_ = true;
	}

	if (last_dot_pos != MAX_STR_LENGTH + 1) {
	    // If dot exists assign name and extension
	    for (size_t i = 0; i < last_dot_pos; i++) { name_ += display_string_[i]; }
	    for (size_t i = last_dot_pos; i < display_string_.size(); i++) { extension_str += display_string_[i]; }
	} else {
	    // Else assign default extension string
	    extension_ = &ProcessedExtension::default_extension_;
	    data_ = action_string;
	    data_set_ = true;
	}

	if (ProcessedExtension::Contains(extension_str)) {
	    extension_ = &(ProcessedExtension::Get(extension_str));
	} else {
	    extension_ = &ProcessedExtension::default_extension_;
	    data_ = action_string;
	    data_set_ = true;
	}

	std::cout << "action_string: " << action_string << std::endl;
	std::cout << "display_string: " << display_string_ << std::endl;
	std::cout << "data: " << data_ << std::endl;
	std::cout << "extension: " << extension_str << std::endl;
    }

    int Execute() {
	if (data_set_) {
	    return extension_->func(data_);
	} else {
	    std::ifstream file(display_string_);
	    if (file.good()) {
		// read the contents of the file into a string
		std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		// run the script with 'arg' as the argument
		return extension_->func(contents);
	    } else {
		return extension_->func(name_);
	    }
	}
    }

    QString Colour() {
	return extension_->colour;
    }
};

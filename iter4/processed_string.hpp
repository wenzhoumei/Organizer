#pragma once
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>

#include <QString>

#include "my_filesystem.hpp"
#include "extension.hpp"

class ProcessedString {
public:
    static void ClearCache() {
	processed_strings.clear();
    }

    static bool Processed(const std::string& str) {
	if (processed_strings.contains(str)) {
	    return true;
	} else {
	    return false;
	}
    }

    static ProcessedString& GetProcessedString(const std::string& str) {
	return processed_strings.at(str);
    }

private:
    //static cache dict
    static inline std::unordered_map<std::string, ProcessedString> processed_strings;

private:
    std::string data_;

    Extension* extension_ = nullptr;

    static constexpr int MAX_STR_LENGTH = 1000;

public:
    ProcessedString(std::string str) {
	if (str.size() > MAX_STR_LENGTH) {
	    std::cerr << "String too big" << std::endl;
	    return;
	}

	size_t last_delimiter = str.find_last_of(Extension::GetDelimiter(Extension::Delimiter::ACTION));

	char delimiter = str[last_delimiter];
	std::string name = str.substr(0, last_delimiter);
	std::string extension = str.substr(last_delimiter + 1);

	if (delimiter == '.') {
	    if (Extension::ContainsProcessedDataExtension(extension)) {
		data_ = str;
		extension_ = Extension::GetProcessedActionExtension(extension);
	    } else {
		data_ = str;
		extension_ = Extension::GetProcessedDefaultExtension();
	    }
	} else if (delimiter == '|') {
	    if (Extension::ContainsProcessedActionExtension(extension)) {
		data_ = name;
		extension_ = Extension::GetProcessedActionExtension(extension);
	    } else {
		data_ = str;
		extension_ = Extension::GetProcessedDefaultExtension();
	    }
	} else if (delimiter == '@') {
	    //TODO
	}

	//std::cout << "string: " << str << std::endl;
	//std::cout << "data: " << data_ << std::endl;
    }

    int Execute() {
	std::string script_path = MyFileSystem::GetActionExtensionsDirectoryPath()/extension_->extension_name;
	std::string command = script_path + " " + data_;
	std::cout << command << std::endl;

	return WEXITSTATUS(std::system(command.c_str()));
    }

    QString Colour() {
	return extension_->colour;
    }
};

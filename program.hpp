#ifndef ACTION_STRING_HPP
#define ACTION_STRING_HPP

#include <QApplication>
#include <QString>

#include <regex>
#include <string>
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

#include "processed_string.hpp"
#include "extension_list.hpp"
//#include "menu_window.hpp"

enum ExitCode { OK = 0 };

class Program {
private:
    std::vector<std::string> current_space_;

private:
    std::filesystem::path root_dir_path_;
    std::filesystem::path globals_dir_path_;

    std::filesystem::path globals_dir_name_ = "globals";
    std::filesystem::path actions_dir_name_ = "actions";
    std::filesystem::path filenames_dir_name_ = "filenames";
    std::filesystem::path colours_dir_name_ = "colours";

    std::string space_file_name_ = "space";
    std::string whitelist_file_name_ = "whitelist";

    std::string reserved_extension_;
    std::string default_extension_;

private:
    QString AssignColour_(const std::string& extension_name);


public:
    Program(std::filesystem::path root_dir_path, std::string reserved_extension, std::string default_extension)
	: root_dir_path_(root_dir_path), reserved_extension_(reserved_extension), default_extension_(default_extension)
    {
	if (!std::filesystem::exists(root_dir_path)) {
	    std::cout << "File doesn't exist" << std::endl;
	    return;
	}

	globals_dir_name_ += '.' + reserved_extension;
	actions_dir_name_ += '.' + reserved_extension;
	filenames_dir_name_ += '.' + reserved_extension;
	colours_dir_name_ += '.' + reserved_extension;

	std::filesystem::current_path(root_dir_path_);
	globals_dir_path_ = root_dir_path / globals_dir_name_;

	ExtensionList extension_list = ExtensionList(globals_dir_path_/actions_dir_name_, globals_dir_path_/filenames_dir_name_, globals_dir_path_/colours_dir_name_, reserved_extension_, default_extension_);

	ProcessedString::SetGlobalDirectory(globals_dir_path_);
	ProcessedString::SetExtensionList(extension_list);

	//ProcessedExtension::Print();
	ProcessedString("hello");
	ProcessedString("hello.link");
	ProcessedString("google.link");
	ProcessedString("what.edit");
	ProcessedString("what.edit,and the data");
    }


public:
    /*
    int Run(int argc, char* argv[]) {
	QApplication app(argc, argv);
	MenuWindow window(8, 33, 300);
	return app.exec();
    }
    */
};
#endif

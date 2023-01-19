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

#include "processed_extension.hpp"
#include "processed_string.hpp"
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

private:
    QString AssignColour_(const std::string& extension_name);


private:
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

public:
    Program(std::string root_dir_path, std::string reserved_extension)
	: root_dir_path_(root_dir_path), reserved_extension_(reserved_extension)
    {
	globals_dir_name_ += '.' + reserved_extension;
	actions_dir_name_ += '.' + reserved_extension;
	filenames_dir_name_ += '.' + reserved_extension;
	colours_dir_name_ += '.' + reserved_extension;

	std::filesystem::current_path(root_dir_path);
	globals_dir_path_ = root_dir_path / globals_dir_name_;
	std::cout << globals_dir_path_ << std::endl;

	ProcessedExtension::LoadExtensions(globals_dir_path_/actions_dir_name_, globals_dir_name_/filenames_dir_name_, globals_dir_name_/colours_dir_name_);
	ProcessedExtension::Print();
	ProcessedString("hello");
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

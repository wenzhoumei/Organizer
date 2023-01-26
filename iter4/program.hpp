#pragma once

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
#include "my_filesystem.hpp"
#include "data_retriever.hpp"
#include "extension.hpp"

#include <stdlib.h>  
#include <thread>  

enum ExitCode { OK = 0 };

class Program {
private:
    static inline std::vector<std::string> current_space_;

private:
    std::string ConsoleMenu_(const std::vector<std::string>& vec) {
	std::cout << "|" << DataRetriever::GetCurrentDirectoryName() << "|" << std::endl;
	// Print the menu
	for (size_t i = 0; i < vec.size(); i++) {
	    std::cout << i + 1 << ": " << vec[i] << std::endl;
	}
	std::cout << "> ";

	// Get the user's choice
	std::string choice;
	std::getline(std::cin, choice);

	return choice;
    }

    void NewOption(std::string filename) {
	    current_space_.push_back(filename);
	    outfile << filename << std::endl;
    }

    std::ofstream outfile;

    void LoadSpace() {
	std::filesystem::path cur_dir_pth = MyFileSystem::GetCurrentDirectoryPath();

	current_space_ = DataRetriever::ReadCurrentOptionListFile();

	outfile.close();
	outfile.open(cur_dir_pth, std::ios::app);

	for (const auto &entry : std::filesystem::directory_iterator(cur_dir_pth)) {
	    std::string filename = entry.path().filename().string();  // get only the filename
	    if (filename.find(Extension::GetReservedExtensionName()) != std::string::npos) {  // check if the filename ends with the extension
		continue;  // skip this file
	    }
	    if (std::find(current_space_.begin(), current_space_.end(), filename) == current_space_.end()) {
		NewOption(filename);
	    }
	}
    }

    static inline std::atomic_bool stop_flag = false;

    static void ProcessStrings_(std::vector<std::string>::iterator start_it) {
	while (!stop_flag) {
	    for (auto it = start_it; it != current_space_.end(); ++it) {
		ProcessedString s(*it);
	    }

	    stop_flag = true;
	}
    }

    static void StartProcessingStrings_(std::vector<std::string>::iterator start_it) {
	stop_flag = true;
	std::thread t1(ProcessStrings_, start_it);
	stop_flag = false;
	t1.detach();
    }

public:
    Program(std::filesystem::path root_directory_path, std::string reserved_extension, std::string default_extension)
    {
	if (!std::filesystem::exists(root_directory_path)) {
	    std::cout << "File doesn't exist" << std::endl;
	    return;
	}

	Extension::SetReservedExtension(reserved_extension);
	MyFileSystem::InitializeFilesystem(root_directory_path, reserved_extension);
	Extension::LoadActionExtensions();
	Extension::LoadDataExtensions();
	if (!Extension::SetDefaultActionExtension(default_extension)) {
	    std::cerr << "Default extension doesn't exist: " << default_extension << std::endl;
	    exit(-1);
	}

	setenv("ACTION_DIR_PATH", MyFileSystem::GetActionExtensionsDirectoryPath().c_str(), 0);

	LoadSpace();
	StartProcessingStrings_(current_space_.begin());

	std::string input = ConsoleMenu_(current_space_);
	if (ProcessedString::Processed(input)) {
	    ProcessedString::GetProcessedString(input);
	} else {
	    ProcessedString inp(input);
	    inp.Execute();
	}
    }

    ~Program() {
	outfile.close();
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

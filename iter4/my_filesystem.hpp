#pragma once

#include <filesystem>
#include <string>
#include <functional>
#include <iostream>
#include <future>

class MyFileSystem {
private:
    static inline std::filesystem::path barebones_actions_directory_path_ = "/home/wenzhou/Barebones";

private:
    static inline std::filesystem::path current_directory_path_;

    static inline std::string reserved_extension_;

    static inline std::filesystem::path action_extensions_directory_path_;
    static inline std::filesystem::path data_extensions_directory_path_;
    static inline std::filesystem::path colours_directory_path_;

    static void CopyDirectory_(const std::filesystem::path& source_dir, const std::filesystem::path& dest_dir) {
	if (!std::filesystem::is_directory(source_dir)) { 
	    std::cerr << source_dir << " is not a directory, aborting..." << std::endl;
	    exit(-1);
	}

	for (const auto& entry : std::filesystem::directory_iterator(source_dir)) {
	    std::filesystem::path file = entry.path();

	    std::filesystem::copy(file, dest_dir / file.filename());
	}
    }

    // If had to create directory, returns false
    // If directory is a file it exists the program
    // Else returns true
    static bool SetDirectory_(std::filesystem::path& member_variable, const std::filesystem::path& path) {
	member_variable = path;

	if (std::filesystem::is_directory(path)) { 
	    return true;
	}
	else if (std::filesystem::exists(path)) {
	    std::cout << path << " is a file, aborting..." << std::endl;
	    exit(-1);
	}
	else {
	    std::cout << path << " does not exist. Creating directory." << std::endl;
	    std::filesystem::create_directory(path);
	    return false;
	}
    }

public:
    static void InitializeFilesystem(std::filesystem::path root_dir_path, const std::string& reserved_extension, const std::string& settings_dir_name="settings", const std::string& action_extensions_dir_name="actions", const std::string& data_extensions_dir_name="extensions", const std::string& colours_dir_name="colours", const std::string& option_list_file_name="space", const std::string& whitelist_file_name="whitelist") {
	if (!ChangeCurrentPath(root_dir_path)) {
	    std::cerr << "Invalid root directory path: " << root_dir_path.string() << std::endl;
	    exit(-1);
	}

	std::filesystem::path settings_directory_path;
	SetDirectory_(settings_directory_path, root_dir_path/(settings_dir_name + "." + reserved_extension));
	if (!SetDirectory_(action_extensions_directory_path_, settings_directory_path/(action_extensions_dir_name + "." + reserved_extension))) {
	    CopyDirectory_(barebones_actions_directory_path_, action_extensions_directory_path_);
	}

	SetDirectory_(data_extensions_directory_path_, settings_directory_path/(data_extensions_dir_name + "." + reserved_extension));
	SetDirectory_(colours_directory_path_, settings_directory_path/(colours_dir_name + "." + reserved_extension));

	GetCurrentOptionListFilePath = [option_list_file_name, reserved_extension]() { return current_directory_path_/(option_list_file_name + "." + reserved_extension); };
	GetCurrentWhitelistFilePath = [whitelist_file_name, reserved_extension]() { return current_directory_path_/(whitelist_file_name + "." + reserved_extension); };

    }

    static bool ChangeCurrentPath(std::filesystem::path pth) {
	if (std::filesystem::is_directory(pth)) {
	    std::filesystem::current_path(pth);
	    current_directory_path_ = pth;
	    return true;
	} else {
	    return false;
	}
    }

    static bool CurrentOptionListFileExists() { 
	if (std::filesystem::is_regular_file(GetCurrentOptionListFilePath())) {
	    return true;
	} if (std::filesystem::exists(GetCurrentOptionListFilePath())) {
	    std::cout << current_directory_path_ << " exists but is not a regular file, aborting..." << std::endl;
	    exit(-1);
	} else {
	    return false;
	}
    }

    static bool CurrentWhitelistFileExists() { 
	if (std::filesystem::is_regular_file(GetCurrentWhitelistFilePath())) {
	    return true;
	} if (std::filesystem::exists(GetCurrentWhitelistFilePath())) {
	    std::cout << current_directory_path_ << " exists but is not a regular file, aborting..." << std::endl;
	    exit(-1);
	} else {
	    return false;
	}
    }

    static inline std::function<std::filesystem::path(void)> GetCurrentOptionListFilePath;
    static inline std::function<std::filesystem::path(void)> GetCurrentWhitelistFilePath;

    static inline std::filesystem::path GetActionExtensionsDirectoryPath() { return action_extensions_directory_path_; }
    static inline std::filesystem::path GetDataExtensionsDirectoryPath() { return data_extensions_directory_path_; }
    static inline std::filesystem::path GetColoursDirectoryPath() { return colours_directory_path_; }
    static inline std::filesystem::path GetCurrentDirectoryPath() { return current_directory_path_; }
};

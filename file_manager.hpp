#pragma once

#include <dirent.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

class FileManager {
    public:
	// Constructor
	FileManager(const std::string& working_directory) : working_directory_(working_directory) {
	    std::filesystem::path current_path = std::filesystem::current_path(); //Getting path
	    std::filesystem::path subdir_path = current_path / working_directory_;

	    std::filesystem::current_path(subdir_path); //Setting path
	}

	// File management functions
	bool createFile(const std::string& filename) {
	    std::ofstream file(filename);
	    if (file.is_open()) {
		file.close();
		return true;
	    } else {
		std::cerr << "Error creating file!" << std::endl;
		return false;
	    }
	}

	bool createExecutableFile(const std::string& filename) {
	    std::ofstream file(filename);
	    if (file.is_open()) {
		file.close();
		std::system(("chmod +x " + filename).c_str());
		return true;
	    } else {
		std::cerr << "Error creating file!" << std::endl;
		return false;
	    }
	}

	bool deleteFile(const std::string& filename) {
	    if (std::remove(filename.c_str()) == 0) {
		return true;
	    } else {
		std::cerr << "Error deleting file!" << std::endl;
		return false;
	    }
	}

	bool renameFile(const std::string& old_name, const std::string& new_name) {
	    if (std::rename(old_name.c_str(), new_name.c_str()) == 0) {
		return true;
	    } else {
		std::cerr << "Error renaming file!" << std::endl;
		return false;
	    }
	}

	std::vector<std::string> getFileNames(const std::string& directory) {
	    std::vector<std::string> file_names;

	    DIR* dir = opendir(directory.c_str());
	    if (dir == nullptr) {
		std::cerr << "Error opening directory: " << directory << std::endl;
		return file_names;
	    }

	    dirent* entry;
	    while ((entry = readdir(dir)) != nullptr) {
		std::string file_name = entry->d_name;
		// Exclude "." and ".." directories
		if (file_name != "." && file_name != "..") {
		    file_names.push_back(file_name);
		}
	    }

	    closedir(dir);
	    return file_names;
	}

	bool copyFile(const std::string& source, const std::string& destination) {
	    std::ifstream src(source, std::ios::binary);
	    std::ofstream dst(destination, std::ios::binary);
	    dst << src.rdbuf();
	    return src && dst;
	}

	// Directory management functions
	bool createDirectory(const std::string& directory_name) {
	    if (std::system(("mkdir " + directory_name).c_str()) == 0) {
		return true;
	    } else {
		std::cerr << "Error creating directory!" << std::endl;
		return false;
	    }
	}

	bool deleteDirectory(const std::string& directory_name) {
	    if (std::system(("rm -r " + directory_name).c_str()) == 0) {
		return true;
	    } else {
		std::cerr << "Error deleting directory!" << std::endl;
		return false;
	    }
	}

	bool renameDirectory(const std::string& old_name, const std::string& new_name) {
	    if (std::rename(old_name.c_str(), new_name.c_str()) == 0) {
		return true;
	    } else {
		std::cerr << "Error renaming directory!" << std::endl;
		return false;
	    }
	}

	bool copyDirectory(const std::string& source, const std::string& destination) {
	    if (std::system(("cp -r " + source + " " + destination).c_str()) == 0) {
		return true;
	    } else {
		std::cerr << "Error copying directory!" << std::endl;
		return false;
	    }
	}

    private:
	std::string working_directory_;
};

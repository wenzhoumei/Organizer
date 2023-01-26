#pragma once
#include "my_filesystem.hpp"

#include <iterator>
#include <fstream>

class DataRetriever {
public:
    static std::vector<std::string> ReadListFile(const std::string& filename) {
	std::vector<std::string> ret;
	std::ifstream file(filename);
	if (!file.good()) {
	    //std::cerr << "File not found: " << filename << std::endl;
	    return ret;
	}

	std::copy(std::istream_iterator<std::string>(file),
		  std::istream_iterator<std::string>(),
		  std::back_inserter(ret));
	file.close();
	return ret;
    }

    static std::vector<std::string> ReadCurrentOptionListFile() {
	return ReadListFile(MyFileSystem::GetCurrentOptionListFilePath());
    }

    static std::vector<std::string> ReadActionDataExtensionsListFile(const std::filesystem::path& action_extension_name) {
	return ReadListFile(MyFileSystem::GetDataExtensionsDirectoryPath()/action_extension_name);
    }

    static std::vector<std::filesystem::path> GetDirectoryFiles(const std::filesystem::path& directory_path);

    static std::vector<std::filesystem::path> GetActionDirectoryFiles() {
	return GetDirectoryFiles(MyFileSystem::GetActionExtensionsDirectoryPath());
    }

    static std::string GetCurrentDirectoryName() {
	return MyFileSystem::GetCurrentDirectoryPath().filename().string();
    }
};

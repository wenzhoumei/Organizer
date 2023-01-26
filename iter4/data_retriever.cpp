#include "data_retriever.hpp"
#include "extension.hpp"

std::vector<std::filesystem::path> DataRetriever::GetDirectoryFiles(const std::filesystem::path& directory_path) {
    std::vector<std::filesystem::path> ret;

    for (const std::filesystem::path& path : std::filesystem::directory_iterator(directory_path)) {
	if (!(path.extension() == Extension::GetReservedExtensionName())) {
	    ret.push_back(path);
	}
    }

    return ret;
}

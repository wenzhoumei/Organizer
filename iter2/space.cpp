#include "space.hpp"

FileSpace::FileSpace(const std::string& directory_path)
    : directory_path(directory_path)
{
    for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
	std::string filename = entry.path().filename();
	actions.push_back(filename);
    }
}

std::vector<ActionString> FileSpace::GetFileContents() {
    std::vector<ActionString> file_contents;
    for (const auto& filename : actions) {
	std::ifstream file(filename);
	std::string content((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
	file_contents.push_back(content);
    }
    return file_contents;
}

void FileSpace::Print() const {
    for (const std::string& filename : actions)
    {
	std::cout << filename << ' ';
    }
    std::cout << std::endl;
}

ExtensionSpace::ExtensionSpace(const std::string& extension, const std::string& directory_path)
    : FileSpace(directory_path), extension(extension)
{
    auto end = std::remove_if(actions.begin(), actions.end(), [&](const std::string& file) {
        return file.substr(file.find_last_of('.') + 1) != extension;
    });
    actions.erase(end, actions.end());
}

void Space::FindMatches(const ActionString& inputText, std::vector<ActionString>& outputArray) const
{
    // Split the input text into tokens
    std::istringstream iss(inputText);
    std::vector<std::string> inputTokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    // Create vectors to store the exact, prefix, and substring matches
    std::vector<std::string> exactMatches;
    std::vector<std::string> prefixMatches;
    std::vector<std::string> substringMatches;

    // Iterate through each item in the search array
    for (const std::string& item : actions)
    {
	// Check if all of the input tokens can be found in the item's text
	bool allTokensFound = true;
	for (const std::string& token : inputTokens)
	{
	    if (item.find(token) == std::string::npos)
	    {
		allTokensFound = false;
		break;
	    }
	}

	// If all of the input tokens were found, determine if the item's text is an exact match,
	// a prefix match, or a substring match, and add the item to the appropriate vector
	if (allTokensFound)
	{
	    if (inputText == item)
	    {
		exactMatches.push_back(item);
	    }
	    else if (item.find(inputTokens[0]) == 0)
	    {
		prefixMatches.push_back(item);
	    }
	    else
	    {
		substringMatches.push_back(item);
	    }
	}
    }

    // Concatenate the exact, prefix, and substring matches vectors and add them to the output array
    outputArray.insert(outputArray.end(), exactMatches.begin(), exactMatches.end());
    outputArray.insert(outputArray.end(), prefixMatches.begin(), prefixMatches.end());
    outputArray.insert(outputArray.end(), substringMatches.begin(), substringMatches.end());
}

/*
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>

class Space {
protected:
    std::vector<std::string> actions;
public:
    virtual bool EditEntry(int i) = 0;
    virtual void Print() const = 0;

    std::string operator[](const int& i) const {
	return actions[i];
    }

    void FindMatches(const std::string& inputText, std::vector<std::string>& outputArray) const
    {
	// Split the input text into tokens
	std::istringstream iss(inputText);
	std::vector<std::string> inputTokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

	// Create vectors to store the exact, prefix, and substring matches
	std::vector<std::string> exactMatches;
	std::vector<std::string> prefixMatches;
	std::vector<std::string> substringMatches;

	// Iterate through each item in the search array
	for (const std::string& item : actions)
	{
	    // Check if all of the input tokens can be found in the item's text
	    bool allTokensFound = true;
	    for (const std::string& token : inputTokens)
	    {
		if (item.find(token) == std::string::npos)
		{
		    allTokensFound = false;
		    break;
		}
	    }

	    // If all of the input tokens were found, determine if the item's text is an exact match,
	    // a prefix match, or a substring match, and add the item to the appropriate vector
	    if (allTokensFound)
	    {
		if (inputText == item)
		{
		    exactMatches.push_back(item);
		}
		else if (item.find(inputTokens[0]) == 0)
		{
		    prefixMatches.push_back(item);
		}
		else
		{
		    substringMatches.push_back(item);
		}
	    }
	}

	// Concatenate the exact, prefix, and substring matches vectors and add them to the output array
	outputArray.insert(outputArray.end(), exactMatches.begin(), exactMatches.end());
	outputArray.insert(outputArray.end(), prefixMatches.begin(), prefixMatches.end());
	outputArray.insert(outputArray.end(), substringMatches.begin(), substringMatches.end());
    }
};

class CustomSpace: Space {
};

class FileSpace: public Space {
protected:
    std::string directory_path;
public:
    FileSpace(const std::string& directory_path)
	: directory_path(directory_path)
    {
	for (const auto& entry : std::filesystem::directory_iterator(directory_path)) {
	    // Get file name
	    std::string filename = entry.path().filename();
	    actions.push_back(filename);
	}
    }

    bool Contains(std::string action) {
	return std::find(actions.begin(), actions.end(), action) != actions.end();
    }

    void Print() const override
    {
	for (const std::string& filename : actions)
	{
	    std::cout << filename << ' ';
	}
	std::cout << std::endl;
    }
};

class ExtensionSpace: public FileSpace {
private:
    std::string extension;

public:
    ExtensionSpace(const std::string& extension, const std::string& directory_path)
	: FileSpace(directory_path), extension(extension)
    {
        auto end = std::remove_if(actions.begin(), actions.end(), [&](const std::string& file) {
            return file.substr(file.find_last_of('.') + 1) != extension;
        });
        actions.erase(end, actions.end());
    }
};
*/

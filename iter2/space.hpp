#ifndef SPACE_HPP
#define SPACE_HPP

#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include "action_string.hpp"

class Space {
protected:
    std::vector<std::string> actions;
public:
    virtual bool EditEntry(int i) = 0;
    virtual void Print() const = 0;

    std::vector<std::string>::iterator begin() { return actions.begin(); }
    std::vector<std::string>::iterator end() { return actions.end(); }
    std::vector<std::string>::const_iterator begin() const { return actions.begin(); }
    std::vector<std::string>::const_iterator end() const { return actions.end(); }

    ActionString operator[](const int& i) const {
	return actions[i];
    }
    int Length() const {
	return actions.size();
    }

    void FindMatches(const ActionString& inputText, std::vector<ActionString>& outputArray) const;
};

class CustomSpace: Space {
};

class FileSpace: public Space {
protected:
    std::string directory_path;
public:
    FileSpace(const std::string& directory_path);

    std::vector<ActionString> GetFileContents();

    bool EditEntry(int i) override;
    void Print() const override;
};

class ExtensionSpace: public FileSpace {
private:
    std::string extension;

public:
    ExtensionSpace(const std::string& extension, const std::string& directory_path);
};

#endif

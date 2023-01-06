#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
#include <vector>

class Space; // forward declaration of Space class

class Action: public std::string
{
public:
    using std::string::string;
    Action(const std::string& s) : std::string(s) {}  // define a constructor that takes a std::string argument

    // Returns the extension of the string (the portion after the last dot)
    // If there is no dot, returns an empty string
    std::string Extension(char delimiter) const;

    // Returns the name of the string (the portion before the last dot)
    // If there is no dot, returns the whole string
    std::string Name(char delimiter) const;

    // Is the string the name of a file in the directory
    bool IsFile() const;

    // Gets string from file
    std::string GetScriptParameter() const;

    // Gets string from file
    Space HasSpace();

    // Reads a file and returns a vector of strings containing the lines of the file.
    //Returns a vector of strings on success, an empty vector on error.
    Space LoadSpace();

    // Gets name of script corresponding to extension
    Action GetScriptName() const;

    // Runs action
    void Run() const;
};

#endif

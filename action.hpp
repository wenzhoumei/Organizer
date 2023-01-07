#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
class Space;

class Action: public std::string
{
    friend class Program;
    // app: function

    static std::unordered_map<std::string, std::function<void(Action)>> extension_functions;
public:
    using std::string::string;
    Action() : std::string() {}  // define a constructor that takes a std::string argument
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

    // Does it have a space
    Space HasSpace() const;

    // Reads a file and returns a vector of strings containing the lines of the file.
    //Returns a vector of strings on success, an empty vector on error.
    Space LoadSpace() const;

    Action GetSpaceName() const;

    // Gets name of script corresponding to extension
    Action GetScriptName() const;

    // Runs action
    void Run() const;
    void Run2() const;
    bool AddToCurrentSpace(Action a) const;
};

namespace std {
  // define a hash function for the Action class
  template <>
  struct hash<Action> {
    std::size_t operator()(const Action& s) const {
      return std::hash<std::string>()(s);
    }
  };
}

#endif

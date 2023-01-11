#ifndef ACTION_STRING_HPP
#define ACTION_STRING_HPP

#include <string>
#include <QString>
#include <unordered_map>

enum Extension { EXTENSION_SCRIPT, SPACE, ADD_TO_SPACE, COLOUR, REMOVE_FROM_SPACE, DELETE_FILE };

class ActionString: public std::string {
    friend class ActionStringProcessor;
private:
    inline static std::unordered_map<enum Extension, std::string> program_extensions_ = { { EXTENSION_SCRIPT, "ext" }, { SPACE, "space" }, { ADD_TO_SPACE, "add" }, { COLOUR, "colour" }, { REMOVE_FROM_SPACE, "remove" }, { DELETE_FILE, "delete" } };

public:
    ActionString() : std::string() {}  // define a constructor that takes a std::string argument
    ActionString(const std::string& s) : std::string(s) {}  // define a constructor that takes a std::string argument
    ActionString(const QString& s)  { ActionString(QString(s).toStdString()); }  // define a constructor that takes a std::string argument

    std::string Extension() const;
    std::string Name() const;
    std::string String() const;
};

#endif

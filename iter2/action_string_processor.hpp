#include <QString>
#include <filesystem>
#include <unordered_map>
#include <random>
#include <sstream>
#include <string>
#include <iomanip>

#include "space.hpp"
#include "action_string.hpp"

class ActionStringProcessor {
private:
    std::string directory_path_;
    inline static std::unordered_map<std::string, QString> extension_colours_;

    QString GenerateRandomColour_() const {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);

	std::stringstream ss;
	ss << "#";
	for (int i = 0; i < 3; i++) {
	    int num = dis(gen);
	    ss << std::uppercase << std::setw(2) << std::setfill('0') << std::hex << num;
	}

	return QString::fromStdString(ss.str());
    }
public:
    ActionStringProcessor(const std::string& directory_path)
    : directory_path_(directory_path)
    {
    }

    void LoadColours() const
    {
	ExtensionSpace extensions(ActionString::program_extensions_[Extension::COLOUR], directory_path_);
	std::vector<ActionString> colour_codes = extensions.GetFileContents();

	for (int i = 0; i < extensions.Length(); i++) {
	    extension_colours_[extensions[i].Name()] = QString::fromStdString(colour_codes[i]);
	}
    }


    QString GetColour(const ActionString& str) const
    {
	if (!extension_colours_.contains(str.Extension())) {
	    std::ofstream file(str.Extension() + "." + ActionString::program_extensions_[Extension::COLOUR]);
	    if (file.is_open()) {
		QString colour = GenerateRandomColour_();
		extension_colours_[str.Extension()] = colour;
		file << colour.toStdString();
		file.close();
	    } else {
		std::cerr << "Unable to open file" << std::endl;
	    }
	}
	return extension_colours_[str.Extension()];
    }

    std::string LoadScripts() const {
	ExtensionSpace extensions(ActionString::program_extensions_[Extension::EXTENSION_SCRIPT], directory_path_);
	std::vector<ActionString> colour_codes = extensions.GetFileContents();

	for (int i = 0; i < extensions.Length(); i++) {
	    std::function<void(Action)> script_func = std::bind(Action::ExecuteActionExtension_, action, std::placeholders::_1);
	    Action::extension_functions[action.Name('.')] = script_func;
	    extension_colours_[extensions[i].Name()] = QString::fromStdString(colour_codes[i]);
	}
    }

    std::string GetScript() const {
	return extension + "." + program_extensions[Extension::EXTENSION_SCRIPT];
    }
};

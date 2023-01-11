#include "action_string_processor.hpp"
void ActionStringProcessor::LoadColours() const
{
    ExtensionSpace extensions(ActionString::program_extensions[Extension::COLOUR], directory_path);
    std::vector<std::string> colour_codes = extensions.GetFileContents();

    for (int i = 0; i < extensions.Length(); i++) {
	extension_colours[extensions[i]] = QString::fromStdString(colour_codes[i]);
    }
}

QString ActionStringProcessor GetColour(const ActionString& str) const
{
    if (extension_colours.contains(str));
}

std::string LoadScripts() const {
    return extension + "." + program_extensions[Extension::EXTENSION_SCRIPT];
}

std::string GetScript() const {
    return extension + "." + program_extensions[Extension::EXTENSION_SCRIPT];
}

#include <string>
#include <unordered_map>

#include "config.hpp"
#include "action.hpp"


namespace config {
    std::unordered_map<Extension, std::string> default_action_extensions;
    std::unordered_map<Action, Action> file_to_action_extension;
};

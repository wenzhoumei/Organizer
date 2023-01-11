#include "action_string.hpp"
std::string ActionString::Extension() const
{
    size_t delimiterPos = this->find_last_of('.');
    if (delimiterPos == std::string::npos)
	return "";
    else
	return this->substr(delimiterPos + 1);
}

std::string ActionString::Name() const
{
    size_t delimiterPos = this->find_last_of('.');
    if (delimiterPos == std::string::npos)
	return *this;
    else
	return this->substr(0, delimiterPos);
}

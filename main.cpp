//#include "tables.hpp"

#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>

void findMatches(const std::string& inputText, const std::vector<std::string>& searchArray, std::vector<std::string>& outputArray)
{
    // Split the input text into tokens
    std::istringstream iss(inputText);
    std::vector<std::string> inputTokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    // Create vectors to store the exact, prefix, and substring matches
    std::vector<std::string> exactMatches;
    std::vector<std::string> prefixMatches;
    std::vector<std::string> substringMatches;

    // Iterate through each item in the search array
    for (const std::string& item : searchArray)
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
// Database class
int main() {
    Database db("database.db");

    db.insertAction("hello.link", "yo.com");
    struct Action a;
    if (db.getAction("hello.link", a)) {
	a.print();
    }
    return 0;
}
*/

#include <fstream>
#include <string>
#include <vector>

/* Reads a file and returns a vector of strings containing the lines of the file.
 *
 * Returns a vector of strings on success, an empty vector on error.
 */
std::vector<std::string> readLinesFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line))
        lines.push_back(line);
    return lines;
}

template <typename T>
void printVector(const std::vector<T>& vec)
{
    for (const T& element : vec)
    {
        std::cout << element << ' ';
    }
    std::cout << std::endl;
}

std::string default_default_extension = "default";
std::string default_extension_extension = "ext";
std::string default_script_name = "script";
enum { SCRIPT_OK = 0, SCRIPT_EXIT = 87 };

class Action: public std::string
{
public:
    using std::string::string;
    Action(const std::string& s) : std::string(s) {}  // define a constructor that takes a std::string argument

    // Returns the extension of the string (the portion after the last dot)
    // If there is no dot, returns an empty string
    std::string Extension() const
    {
        size_t dotPos = this->find_last_of('.');
        if (dotPos == std::string::npos)
            return "";
        else
            return this->substr(dotPos + 1);
    }

    // Returns the name of the string (the portion before the last dot)
    // If there is no dot, returns the whole string
    std::string Name() const
    {
        size_t dotPos = this->find_last_of('.');
        if (dotPos == std::string::npos)
            return *this;
        else
            return this->substr(0, dotPos);
    }

    std::string GetContents() const {
	std::ifstream file(*this);
	if (file.good()) {
	    // read the contents of the file into a string
	    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	    // close the file
	    file.close();

	    // run the script with 'arg' as the argument
	    return contents;
	} else {
	    return "";
	}
    }

    Action GetScriptName() const {
	    return this->Extension() + "." + default_extension_extension;
    }

    void Run() const {
	Action script_name = this->GetScriptName();

	std::string command;


	// if script doesn't exist exit the function
	if (script_name.GetContents() == "") {
	    command = "./" + default_script_name + "." + default_default_extension + " \"" + *this + "\"";
	} else {
	    std::string file_str = this->GetContents();

	    // if file doesn't exist set arg to action name else set it to file_str
	    std::string arg = file_str == "" ? this->Name(): file_str;

	    command = "./" + script_name + " " + arg;
	}

	std::cout << command << std::endl;

	// run command and store exit status code
	int result = WEXITSTATUS(std::system(command.c_str()));
	if (result == SCRIPT_EXIT) {
	    exit(0);
	} else if (result != SCRIPT_OK) {
	    std::cerr << "Error: script returned non-zero exit code: " << result << std::endl;
	}
    }
};

/*
// findMatches
int main() {
    std::vector<std::string> search = readLinesFromFile("file.txt");
    std::vector<std::string> found;
    findMatches("o w c", search, found);
    printVector(search);
    printVector(found);


    return 0;
}
*/

#include <filesystem>

int main() {
    // Set working directory
    std::filesystem::current_path("/home/wenzhou/Organizer/files");

    while (true) {
	std::cout << "> ";
	Action input;
	getline(std::cin, input);

	input.Run();
    }
}

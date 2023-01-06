//#include "tables.hpp"

#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include "action.hpp"

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
#include "config.hpp"

int main() {
    config::SetDefaults();
    // Set working directory
    std::filesystem::current_path(config::working_directory);

    while (true) {
	std::cout << "> ";
	Action input;


	getline(std::cin, input);

	if (input == Action("q")) {
	    break;
	}

	input.Run();
    }
}

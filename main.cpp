//#include "tables.hpp"

#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include "program.hpp"

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
    Program program("/home/wenzhou/Organizer/files");
    program.SetDefaults();
    program.LoadDefaultScripts();
    program.MainLoop();
}

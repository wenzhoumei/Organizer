//#include "tables.hpp"

#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include "program.hpp"

#include "gui.hpp"

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
#include <QApplication>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    
    MenuWindow window(8, 33, 300);

    /*
    for (int i = 0; i < 7; ++i) {
	window->NewColumn(generateRandomColor());
    }
    */

    return app.exec();
}

/*
int main() {
    // Set working directory
    Program program("/home/wenzhou/Organizer/files");
    program.SetDefaults();
    program.LoadDefaultScripts();
    program.MainLoop();
}
*/

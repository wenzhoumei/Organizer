#include "tables.hpp"

#include <vector>
#include <sstream>
#include <iterator>

void findMatches(const std::string& inputText, const std::vector<std::string>& searchArray, std::vector<std::string>& outputArray)
{
    // Split the input text into tokens
    std::istringstream iss(inputText);
    std::vector<std::string> inputTokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

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

        // If all of the input tokens were found, add the item to the output array
        if (allTokensFound)
        {
            outputArray.push_back(item);
        }
    }
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

template <typename T>
void printVector(const std::vector<T>& vec)
{
    for (const T& element : vec)
    {
        std::cout << element << ' ';
    }
    std::cout << std::endl;
}

// findMatches
int main() {
    std::vector<std::string> search = { "vec", "yo you", "never yo", "ayo" };
    std::vector<std::string> found;
    findMatches("yo", search, found);
    printVector(search);
    printVector(found);

    return 0;
}

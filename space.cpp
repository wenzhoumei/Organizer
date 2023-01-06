#include "space.hpp"
#include <iostream>
#include <iterator>
#include <sstream>

void Space::findMatches(const Action& inputText, Space& outputArray) const
{
    // Split the input text into tokens
    std::istringstream iss(inputText);
    std::vector<std::string> inputTokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    // Create vectors to store the exact, prefix, and substring matches
    std::vector<Action> exactMatches;
    std::vector<Action> prefixMatches;
    std::vector<Action> substringMatches;

    // Iterate through each item in the search array
    for (const std::string& item : *this)
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

void Space::RunAll() const
{
    for (const Action& element : *this)
    {
	element.Run();
    }
}

void Space::Print() const
{
    for (const Action& element : *this)
    {
	std::cout << element << ' ';
    }
    std::cout << std::endl;
}

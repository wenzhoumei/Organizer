#ifndef SPACE_HPP
#define SPACE_HPP

#include "action.hpp"
#include <vector>
#include <string>

class Space: public std::vector<Action> {
public:
    using std::vector<Action>::vector;

    void findMatches(const Action& inputText, Space& outputArray) const;
    void RunAll() const;
    void Print() const;
    //combine spaces
};


#endif

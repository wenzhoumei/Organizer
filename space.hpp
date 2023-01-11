#ifndef SPACE_HPP
#define SPACE_HPP

#include "action.hpp"
#include <vector>
#include <string>

class Space: public std::vector<Action> {
private:
    friend class Action;
    static Space global_space;
    static Space current_space;
public:
    using std::vector<Action>::vector;

    void findMatches(const Action& inputText, Space& outputArray) const;
    void RunAll() const;
    void Print() const;

    bool ContainsAction(Action a) const;
    //combine spaces
};


#endif

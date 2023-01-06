#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

class Action;
class Space;
#include "action.hpp"
#include "space.hpp"

class Navigation {
    static Space global_space_;
private:
    Action current_action_;
    Space current_space_;
    Space history_space_;
public:
    Navigation();
    void Add();
    void Move();
};

#endif

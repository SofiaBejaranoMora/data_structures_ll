#pragma once
#include <SFML/Graphics.hpp>
#include "UserTree.h"
#include "WindowsUser.h"

using namespace sf;

class windowsMenu
{
private:
    VideoMode mode;
    UserTree userTree;

public:
    windowsMenu();
    void run();
};

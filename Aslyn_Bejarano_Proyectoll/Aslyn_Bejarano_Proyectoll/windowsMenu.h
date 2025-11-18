#pragma once

#include <SFML/Graphics.hpp>
#include "UserTree.h"
#include "FileManager.h"

using namespace sf;
using namespace std;

class windowsMenu
{
private:
    VideoMode mode;
    UserTree* userTree;
    FileManager fileManager;
    bool datosCargados;

    void cargarUserTreeSiEsNecesario();

public:
    windowsMenu();
    void run();
};

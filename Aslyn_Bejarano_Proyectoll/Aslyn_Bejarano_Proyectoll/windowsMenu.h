#pragma once

#include <SFML/Graphics.hpp>
#include "UserTree.h"
#include "Graph.h"
#include "FileManager.h"

using namespace sf;
using namespace std;

class windowsMenu
{
private:
    VideoMode mode;
    UserTree* userTree;
    Graph* graph;
    FileManager fileManager;
    bool datosCargadosUsers;
    bool datosCargadosGraph;

    void cargarUserTreeSiEsNecesario();
    void cargarGraphSiEsNecesario();

public:
    windowsMenu();
    ~windowsMenu();
    void run();
};

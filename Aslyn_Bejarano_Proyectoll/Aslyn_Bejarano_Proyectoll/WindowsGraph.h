#pragma once

#include <SFML/Graphics.hpp>
#include "Graph.h"

using namespace sf;
using namespace std;

class WindowsGraph
{
private:
    VideoMode mode;

public:
    WindowsGraph();
    void run(const Graph& graph);
};

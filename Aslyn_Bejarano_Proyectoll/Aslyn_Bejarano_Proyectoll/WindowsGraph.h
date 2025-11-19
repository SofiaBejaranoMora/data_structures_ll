#pragma once

#include <SFML/Graphics.hpp>
#include "Graph.h"
#include "Button.h"

using namespace sf;

class WindowsGraph {
private:
    VideoMode mode;

public:
    WindowsGraph();
    void run(const Graph& graph);
};

#pragma once

#include <SFML/Graphics.hpp>
#include "Graph.h"
#include "Button.h"

using namespace sf;

enum class GraphWindowMode {
    Rutas,
    Dijkstra
};

class WindowsGraph {
private:
    VideoMode mode;

    void runRutas(const Graph& graph);
    void runDijkstra(const Graph& graph);

public:
    WindowsGraph();
    void run(const Graph& graph, GraphWindowMode mode);
};

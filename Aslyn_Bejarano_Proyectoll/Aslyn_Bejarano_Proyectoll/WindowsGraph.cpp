#include "WindowsGraph.h"
#include "Button.h"

#include <vector>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace sf;
using namespace std;

namespace
{
    void drawStopNode(RenderWindow& window, const Font& font,
        const string& label, const Vector2f& position)
    {
        float nodeWidth = 160.f;
        float nodeHeight = 50.f;

        Color morado(62, 27, 116);
        Color lila(194, 189, 224);

        RectangleShape rect;
        rect.setSize(Vector2f(nodeWidth, nodeHeight));
        rect.setFillColor(morado);
        rect.setOrigin(nodeWidth / 2.f, nodeHeight / 2.f);
        rect.setPosition(position);
        window.draw(rect);

        Text text;
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(26);
        text.setFillColor(lila);

        FloatRect tb = text.getLocalBounds();
        text.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
        text.setPosition(position);

        window.draw(text);
    }

    void drawEdgeLine(RenderWindow& window, const Vector2f& from, const Vector2f& to)
    {
        Vertex line[] = {
            Vertex(from, Color::Black),
            Vertex(to, Color::Black)
        };
        window.draw(line, 2, Lines);
    }

    void drawEdgeWeight(RenderWindow& window, const Font& font,
        const Vector2f& from, const Vector2f& to,
        double weight, const Color& color)
    {
        Vector2f mid((from.x + to.x) / 2.f, (from.y + to.y) / 2.f - 18.f);

        Text text;
        text.setFont(font);

        ostringstream oss;
        oss << fixed << setprecision(2) << weight;
        text.setString(oss.str());
        text.setCharacterSize(20);
        text.setFillColor(color);

        FloatRect lb = text.getLocalBounds();
        text.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
        text.setPosition(mid);

        window.draw(text);
    }

    double findEdgeWeight(const Graph& graph, int originId, int destinationId)
    {
        const vector<Stop*>& stops = graph.getStops();
        const vector<vector<pair<int, double>>>& adjacency = graph.getAdjacencyList();

        int n = (int)stops.size();
        int originIndex = -1;
        int destIndex = -1;

        for (int i = 0; i < n; ++i)
        {
            if (stops[i] && stops[i]->getId() == originId)
                originIndex = i;
            if (stops[i] && stops[i]->getId() == destinationId)
                destIndex = i;
        }

        if (originIndex == -1 || destIndex == -1)
            return 0.0;

        for (const auto& neighbor : adjacency[originIndex])
        {
            if (neighbor.first == destIndex)
                return neighbor.second;
        }

        for (const auto& neighbor : adjacency[destIndex])
        {
            if (neighbor.first == originIndex)
                return neighbor.second;
        }

        return 0.0;
    }

    void drawOriginalGraph(RenderWindow& window, const Graph& graph,
        const Font& font, const FloatRect& areaBounds,
        bool showWeights)
    {
        const vector<Stop*>& stops = graph.getStops();
        size_t n = stops.size();
        if (n == 0)
            return;

        float radius = std::min(areaBounds.width, areaBounds.height) * 0.35f;
        Vector2f center(areaBounds.left + areaBounds.width / 2.f,
            areaBounds.top + areaBounds.height / 2.f);

        vector<Vector2f> positions(n);
        const float PI = 3.14159265359f;

        for (size_t i = 0; i < n; ++i)
        {
            float angle = 2.f * PI * static_cast<float>(i) / static_cast<float>(n) - PI / 2.f;
            positions[i].x = center.x + radius * std::cos(angle);
            positions[i].y = center.y + radius * std::sin(angle);
        }

        const vector<vector<pair<int, double>>>& adjacency = graph.getAdjacencyList();

        for (size_t i = 0; i < n; ++i)
        {
            for (const auto& neighbor : adjacency[i])
            {
                int j = neighbor.first;
                if (j <= static_cast<int>(i))
                    continue;

                Vector2f from = positions[i];
                Vector2f to = positions[j];

                drawEdgeLine(window, from, to);

                if (showWeights)
                {
                    drawEdgeWeight(window, font, from, to,
                        neighbor.second, Color(62, 27, 116));
                }
            }
        }

        for (size_t i = 0; i < n; ++i)
        {
            if (stops[i])
                drawStopNode(window, font, stops[i]->getName(), positions[i]);
        }
    }

    void drawTraversalGraph(RenderWindow& window, const Graph& graph,
        const Font& font, const vector<Stop*>& order,
        const FloatRect& areaBounds, bool showWeights)
    {
        size_t n = order.size();
        if (n == 0)
            return;

        float usableWidth = areaBounds.width * 0.8f;
        float spacing = (n > 1) ? usableWidth / static_cast<float>(n - 1) : 0.f;
        float startX = areaBounds.left + (areaBounds.width - usableWidth) / 2.f;
        float y = areaBounds.top + areaBounds.height / 2.f;

        vector<Vector2f> positions(n);
        for (size_t i = 0; i < n; ++i)
        {
            positions[i].x = startX + spacing * static_cast<float>(i);
            positions[i].y = y;
        }

        if (n >= 2)
        {
            for (size_t i = 0; i < n - 1; ++i)
            {
                Vector2f from = positions[i];
                Vector2f to = positions[i + 1];

                drawEdgeLine(window, from, to);

                if (showWeights)
                {
                    int originId = order[i]->getId();
                    int destId = order[i + 1]->getId();
                    double w = findEdgeWeight(graph, originId, destId);
                    drawEdgeWeight(window, font, from, to, w, Color(62, 27, 116));
                }
            }
        }

        for (size_t i = 0; i < n; ++i)
        {
            if (order[i])
                drawStopNode(window, font, order[i]->getName(), positions[i]);
        }
    }
}

WindowsGraph::WindowsGraph()
{
    mode = VideoMode::getDesktopMode();
}

void WindowsGraph::run(const Graph& graph)
{
    RenderWindow window(mode, "Grafo", Style::Fullscreen);
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("segoeprb.ttf"))
        return;

    Color fondo(237, 237, 237);
    Color lila(194, 189, 224);
    Color morado(62, 27, 116);

    float buttonWidth = 260.f;
    float buttonHeight = 90.f;
    float spacing = 40.f;

    float totalWidth = 4.f * buttonWidth + 3.f * spacing;
    float startX = (mode.width - totalWidth) / 2.f;
    float row1Y = 20.f;

    Button botonGrafo(buttonWidth, buttonHeight, 40, font);
    botonGrafo.build("Grafo", Vector2f(startX, row1Y), "lila");

    Button botonPrim(buttonWidth, buttonHeight, 40, font);
    botonPrim.build("Prim", Vector2f(startX + (buttonWidth + spacing) * 1.f, row1Y), "lila");

    Button botonAncho(buttonWidth, buttonHeight, 40, font);
    botonAncho.build("Ancho", Vector2f(startX + (buttonWidth + spacing) * 2.f, row1Y), "lila");

    Button botonProfundo(buttonWidth, buttonHeight, 40, font);
    botonProfundo.build("Profundo", Vector2f(startX + (buttonWidth + spacing) * 3.f, row1Y), "lila");

    float secondRowY = row1Y + buttonHeight + 15.f;

    float costWidth = buttonWidth + 40.f;
    float costHeight = 70.f;

    RectangleShape costBox;
    costBox.setSize(Vector2f(costWidth, costHeight));
    costBox.setFillColor(lila);
    costBox.setOutlineThickness(2.f);
    costBox.setOutlineColor(morado);

    float costX = botonGrafo.getShape().getPosition().x + (buttonWidth - costWidth) / 2.f;
    costBox.setPosition(Vector2f(costX, secondRowY));

    Text costText;
    costText.setFont(font);
    costText.setCharacterSize(32);
    costText.setFillColor(morado);

    auto positionCostText = [&]()
        {
            FloatRect b = costText.getLocalBounds();
            costText.setOrigin(b.left + b.width / 2.f, b.top + b.height / 2.f);
            costText.setPosition(
                costBox.getPosition().x + costBox.getSize().x / 2.f,
                costBox.getPosition().y + costBox.getSize().y / 2.f
            );
        };

    auto setCostoEmpty = [&]()
        {
            costText.setString("Costo total: -");
            positionCostText();
        };

    auto setCostoValue = [&](double value)
        {
            ostringstream oss;
            oss << fixed << setprecision(2) << value;
            costText.setString("Costo total: " + oss.str());
            positionCostText();
        };

    setCostoEmpty();

    Button botonVolver(buttonWidth, buttonHeight, 32, font);
    botonVolver.build(
        "Volver Menu",
        Vector2f(botonProfundo.getShape().getPosition().x, secondRowY),
        "lila"
    );

    float margin = 20.f;
    float areaTop = costBox.getPosition().y + costBox.getSize().y + 25.f;

    RectangleShape areaGraph;
    areaGraph.setPosition(Vector2f(margin, areaTop));
    areaGraph.setSize(Vector2f(mode.width - 2.f * margin, mode.height - areaTop - margin));
    areaGraph.setFillColor(Color::White);
    areaGraph.setOutlineThickness(3.f);
    areaGraph.setOutlineColor(Color::Black);

    bool hoverGrafo = false;
    bool hoverPrim = false;
    bool hoverAncho = false;
    bool hoverProfundo = false;
    bool hoverVolver = false;

    enum class GraphViewMode { Original, Prim, BFS, DFS };
    GraphViewMode currentMode = GraphViewMode::Original;

    Graph* primGraph = nullptr;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
            {
                if (primGraph)
                {
                    delete primGraph;
                    primGraph = nullptr;
                }
                window.close();
            }

            auto handleButtonEvent = [&](Button& button, bool& hoverFlag) -> bool
                {
                    bool clicked = false;
                    FloatRect bounds = button.getShape().getGlobalBounds();

                    if (event.type == Event::MouseMoved)
                    {
                        Vector2i mousePos = Mouse::getPosition(window);
                        bool nowHover = bounds.contains(
                            static_cast<float>(mousePos.x),
                            static_cast<float>(mousePos.y)
                        );
                        hoverFlag = nowHover;

                        if (nowHover)
                            button.getShape().setFillColor(button.getColorHover());
                        else
                            button.getShape().setFillColor(button.getColorNormal());
                    }

                    if (event.type == Event::MouseButtonPressed &&
                        event.mouseButton.button == Mouse::Left)
                    {
                        Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
                        if (bounds.contains(
                            static_cast<float>(mousePos.x),
                            static_cast<float>(mousePos.y)))
                        {
                            button.getShape().setFillColor(button.getColorPressed());
                        }
                    }

                    if (event.type == Event::MouseButtonReleased &&
                        event.mouseButton.button == Mouse::Left)
                    {
                        Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
                        bool inside = bounds.contains(
                            static_cast<float>(mousePos.x),
                            static_cast<float>(mousePos.y));

                        button.getShape().setFillColor(
                            hoverFlag ? button.getColorHover()
                            : button.getColorNormal()
                        );

                        if (inside)
                            clicked = true;
                    }

                    return clicked;
                };

            bool clickedGrafo = handleButtonEvent(botonGrafo, hoverGrafo);
            bool clickedPrim = handleButtonEvent(botonPrim, hoverPrim);
            bool clickedAncho = handleButtonEvent(botonAncho, hoverAncho);
            bool clickedProfundo = handleButtonEvent(botonProfundo, hoverProfundo);
            bool clickedVolver = handleButtonEvent(botonVolver, hoverVolver);

            if (clickedGrafo)
            {
                currentMode = GraphViewMode::Original;
                setCostoEmpty();
            }

            if (clickedPrim)
            {
                currentMode = GraphViewMode::Prim;

                if (primGraph)
                {
                    delete primGraph;
                    primGraph = nullptr;
                }

                primGraph = graph.getTreePrim();
                double totalCost = graph.getTreePrimTotalCost();
                setCostoValue(totalCost);
            }

            if (clickedAncho)
            {
                currentMode = GraphViewMode::BFS;
                setCostoEmpty();
            }

            if (clickedProfundo)
            {
                currentMode = GraphViewMode::DFS;
                setCostoEmpty();
            }

            if (clickedVolver)
            {
                if (primGraph)
                {
                    delete primGraph;
                    primGraph = nullptr;
                }
                window.close();
            }
        }

        window.clear(fondo);

        window.draw(botonGrafo.getShape());
        window.draw(botonGrafo.getText());

        window.draw(botonPrim.getShape());
        window.draw(botonPrim.getText());

        window.draw(botonAncho.getShape());
        window.draw(botonAncho.getText());

        window.draw(botonProfundo.getShape());
        window.draw(botonProfundo.getText());

        window.draw(botonVolver.getShape());
        window.draw(botonVolver.getText());

        window.draw(costBox);
        window.draw(costText);

        window.draw(areaGraph);

        FloatRect areaBounds = areaGraph.getGlobalBounds();

        if (currentMode == GraphViewMode::Original)
        {
            drawOriginalGraph(window, graph, font, areaBounds, true);
        }
        else if (currentMode == GraphViewMode::Prim)
        {
            if (primGraph != nullptr)
                drawOriginalGraph(window, *primGraph, font, areaBounds, true);
        }
        else if (currentMode == GraphViewMode::BFS)
        {
            vector<Stop*> order = graph.breadthFirstTraversal();
            drawTraversalGraph(window, graph, font, order, areaBounds, false);
        }
        else if (currentMode == GraphViewMode::DFS)
        {
            vector<Stop*> order = graph.depthFirstTraversal();
            drawTraversalGraph(window, graph, font, order, areaBounds, false);
        }

        window.display();
    }
}

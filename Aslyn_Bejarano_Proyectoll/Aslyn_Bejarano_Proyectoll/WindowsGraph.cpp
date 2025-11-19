#include "WindowsGraph.h"
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace sf;
using namespace std;

struct TextField {
    RectangleShape box;
    Text label;
    Text valueText;
    string value;
    bool active;

    TextField() : active(false) {}
};

static void configurarCampo(TextField& campo,
    const string& etiqueta,
    const Font& font,
    const Vector2f& position,
    const Vector2f& size) {
    Color lila(194, 189, 224);
    Color morado(62, 27, 116);

    campo.box.setSize(size);
    campo.box.setPosition(position);
    campo.box.setFillColor(lila);
    campo.box.setOutlineThickness(2.f);
    campo.box.setOutlineColor(morado);

    campo.label.setFont(font);
    campo.label.setString(etiqueta);
    campo.label.setCharacterSize(18);
    campo.label.setFillColor(morado);

    FloatRect lb = campo.label.getLocalBounds();
    float centerX = position.x + size.x / 2.f;
    float labelY = position.y - 24.f;
    campo.label.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
    campo.label.setPosition(centerX, labelY);

    campo.value.clear();
    campo.valueText.setFont(font);
    campo.valueText.setCharacterSize(20);
    campo.valueText.setFillColor(morado);
    campo.valueText.setString("");

    FloatRect vb = campo.valueText.getLocalBounds();
    campo.valueText.setOrigin(vb.left, vb.top + vb.height / 2.f);
    campo.valueText.setPosition(position.x + 10.f, position.y + size.y / 2.f);

    campo.active = false;
}

static void manejarTextoCampo(TextField& campo, Uint32 unicode) {
    if (!campo.active)
        return;

    if (unicode == 8) {
        if (!campo.value.empty()) {
            campo.value.pop_back();
            campo.valueText.setString(campo.value);
        }
        return;
    }

    if (unicode < 32 || unicode > 126)
        return;

    char c = static_cast<char>(unicode);
    campo.value.push_back(c);
    campo.valueText.setString(campo.value);
}

static double obtenerPesoArista(const Graph& graph,
    const Stop* a,
    const Stop* b) {
    const vector<Stop*>& stops = graph.getStops();
    const vector<vector<pair<int, double>>>& adj = graph.getAdjacencyList();

    int ia = -1;
    int ib = -1;

    for (int i = 0; i < (int)stops.size(); i++) {
        if (stops[i] == a) ia = i;
        if (stops[i] == b) ib = i;
    }

    if (ia == -1 || ib == -1)
        return 0.0;

    for (const auto& edge : adj[ia]) {
        if (edge.first == ib)
            return edge.second;
    }

    return 0.0;
}

WindowsGraph::WindowsGraph() {
    mode = VideoMode::getDesktopMode();
}

void WindowsGraph::run(const Graph& graph, GraphWindowMode windowMode) {
    if (windowMode == GraphWindowMode::Rutas) {
        runRutas(graph);
    }
    else {
        runDijkstra(graph);
    }
}

/* =====================  MODO RUTAS (GRAFO/PRIM/ANCHO/PROFUNDO)  ===================== */

void WindowsGraph::runRutas(const Graph& graph) {
    RenderWindow window(
        mode,
        "Grafo",
        Style::Fullscreen
    );
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("segoeprb.ttf"))
        return;

    Color fondo(237, 237, 237);
    Color lila(194, 189, 224);
    Color morado(62, 27, 116);

    float marginTop = 30.f;
    float marginSide = 40.f;

    float buttonWidth = 260.f;
    float buttonHeight = 70.f;
    float spacing = 25.f;

    Button botonGrafo(buttonWidth, buttonHeight, 30, font);
    botonGrafo.build("Grafo",
        Vector2f(marginSide, marginTop),
        "lila");

    Button botonPrim(buttonWidth, buttonHeight, 30, font);
    botonPrim.build("Prim",
        Vector2f(marginSide + (buttonWidth + spacing) * 1, marginTop),
        "lila");

    Button botonAncho(buttonWidth, buttonHeight, 30, font);
    botonAncho.build("Ancho",
        Vector2f(marginSide + (buttonWidth + spacing) * 2, marginTop),
        "lila");

    Button botonProfundo(buttonWidth, buttonHeight, 30, font);
    botonProfundo.build("Profundo",
        Vector2f(marginSide + (buttonWidth + spacing) * 3, marginTop),
        "lila");

    Button botonVolver(buttonWidth, buttonHeight, 30, font);
    float volverX = mode.width - marginSide - buttonWidth;
    botonVolver.build("Volver menu",
        Vector2f(volverX, marginTop),
        "lila");

    float costoWidth = buttonWidth;
    float costoHeight = 60.f;

    RectangleShape boxCosto;
    boxCosto.setSize(Vector2f(costoWidth, costoHeight));
    boxCosto.setPosition(Vector2f(marginSide, marginTop + buttonHeight + 10.f));
    boxCosto.setFillColor(lila);
    boxCosto.setOutlineThickness(2.f);
    boxCosto.setOutlineColor(morado);

    Text costoText;
    costoText.setFont(font);
    costoText.setCharacterSize(24);
    costoText.setFillColor(morado);
    costoText.setString("Costo total: -");
    {
        FloatRect cb = costoText.getLocalBounds();
        costoText.setOrigin(cb.left + cb.width / 2.f, cb.top + cb.height / 2.f);
        costoText.setPosition(
            boxCosto.getPosition().x + costoWidth / 2.f,
            boxCosto.getPosition().y + costoHeight / 2.f
        );
    }

    float areaTop = boxCosto.getPosition().y + costoHeight + 40.f;

    RectangleShape areaGraph;
    areaGraph.setPosition(Vector2f(marginSide, areaTop));
    areaGraph.setSize(Vector2f(mode.width - 2.f * marginSide,
        mode.height - areaTop - 30.f));
    areaGraph.setFillColor(Color::White);
    areaGraph.setOutlineThickness(3.f);
    areaGraph.setOutlineColor(Color::Black);

    bool hoverGrafo = false;
    bool hoverPrim = false;
    bool hoverAncho = false;
    bool hoverProfundo = false;
    bool hoverVolver = false;

    vector<Stop*> bfsOrder = graph.breadthFirstTraversal();
    vector<Stop*> dfsOrder = graph.depthFirstTraversal();

    enum class ViewMode { Original, Prim, BFS, DFS };
    ViewMode viewMode = ViewMode::Original;

    auto handleButton = [&](Event& event, Button& button, bool& hoverFlag) -> bool {
        bool clicked = false;
        FloatRect bounds = button.getShape().getGlobalBounds();

        if (event.type == Event::MouseMoved) {
            Vector2i mousePos = Mouse::getPosition(window);
            bool nowHover = bounds.contains(static_cast<float>(mousePos.x),
                static_cast<float>(mousePos.y));
            hoverFlag = nowHover;

            if (nowHover)
                button.getShape().setFillColor(button.getColorHover());
            else
                button.getShape().setFillColor(button.getColorNormal());
        }

        if (event.type == Event::MouseButtonPressed &&
            event.mouseButton.button == Mouse::Left) {

            Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            if (bounds.contains(static_cast<float>(mousePos.x),
                static_cast<float>(mousePos.y))) {
                button.getShape().setFillColor(button.getColorPressed());
            }
        }

        if (event.type == Event::MouseButtonReleased &&
            event.mouseButton.button == Mouse::Left) {

            Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            bool inside = bounds.contains(static_cast<float>(mousePos.x),
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

    auto actualizarCosto = [&](const string& texto) {
        costoText.setString(texto);
        FloatRect cb = costoText.getLocalBounds();
        costoText.setOrigin(cb.left + cb.width / 2.f,
            cb.top + cb.height / 2.f);
        costoText.setPosition(
            boxCosto.getPosition().x + costoWidth / 2.f,
            boxCosto.getPosition().y + costoHeight / 2.f
        );
        };

    auto dibujarNodo = [&](const string& label, const Vector2f& position) {
        float nodeWidth = 150.f;
        float nodeHeight = 50.f;

        RectangleShape rect;
        rect.setSize(Vector2f(nodeWidth, nodeHeight));
        rect.setOrigin(nodeWidth / 2.f, nodeHeight / 2.f);
        rect.setPosition(position);
        rect.setFillColor(morado);

        Text text;
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(22);
        text.setFillColor(Color(194, 189, 224));

        FloatRect tb = text.getLocalBounds();
        text.setOrigin(tb.left + tb.width / 2.f,
            tb.top + tb.height / 2.f);
        text.setPosition(position);

        window.draw(rect);
        window.draw(text);
        };

    auto dibujarGrafo = [&](const Graph& g) {
        const vector<Stop*>& stops = g.getStops();
        const vector<vector<pair<int, double>>>& adj = g.getAdjacencyList();

        int n = (int)stops.size();
        if (n == 0)
            return;

        vector<Vector2f> positions;
        positions.resize(n);

        Vector2f center(
            areaGraph.getPosition().x + areaGraph.getSize().x / 2.f,
            areaGraph.getPosition().y + areaGraph.getSize().y / 2.f
        );
        float radius = min(areaGraph.getSize().x, areaGraph.getSize().y) / 3.f;

        for (int i = 0; i < n; ++i) {
            float angle = 2.f * 3.1415926f * static_cast<float>(i) / static_cast<float>(n);
            float x = center.x + radius * cos(angle);
            float y = center.y + radius * sin(angle);
            positions[i] = Vector2f(x, y);
        }

        for (int i = 0; i < n; ++i) {
            for (const auto& edge : adj[i]) {
                int j = edge.first;
                if (j <= i)
                    continue;

                Vector2f a = positions[i];
                Vector2f b = positions[j];

                Vertex line[] = {
                    Vertex(a, Color::Black),
                    Vertex(b, Color::Black)
                };
                window.draw(line, 2, Lines);

                double w = edge.second;
                ostringstream ss;
                ss.setf(ios::fixed);
                ss << setprecision(2) << w;

                Text weightText;
                weightText.setFont(font);
                weightText.setCharacterSize(18);
                weightText.setFillColor(morado);
                weightText.setString(ss.str());

                Vector2f mid(
                    (a.x + b.x) / 2.f,
                    (a.y + b.y) / 2.f - 18.f
                );
                FloatRect wb = weightText.getLocalBounds();
                weightText.setOrigin(wb.left + wb.width / 2.f,
                    wb.top + wb.height / 2.f);
                weightText.setPosition(mid);

                window.draw(weightText);
            }
        }

        for (int i = 0; i < n; ++i) {
            if (stops[i] != nullptr) {
                dibujarNodo(stops[i]->getName(), positions[i]);
            }
        }
        };

    auto dibujarCaminoLineal = [&](const vector<Stop*>& lista) {
        size_t count = lista.size();
        if (count == 0)
            return;

        float left = areaGraph.getPosition().x + 80.f;
        float right = areaGraph.getPosition().x + areaGraph.getSize().x - 80.f;
        float y = areaGraph.getPosition().y + areaGraph.getSize().y / 2.f;

        float step = 0.f;
        if (count > 1) {
            step = (right - left) / static_cast<float>(count - 1);
        }

        vector<Vector2f> nodePositions;
        nodePositions.reserve(count);

        for (size_t i = 0; i < count; ++i) {
            float x = left + step * static_cast<float>(i);
            nodePositions.push_back(Vector2f(x, y));
        }

        for (size_t i = 0; i + 1 < count; ++i) {
            Vertex line[] = {
                Vertex(nodePositions[i], Color::Black),
                Vertex(nodePositions[i + 1], Color::Black)
            };
            window.draw(line, 2, Lines);

            double w = obtenerPesoArista(graph, lista[i], lista[i + 1]);
            ostringstream ss;
            ss.setf(ios::fixed);
            ss << setprecision(2) << w;

            Text weightText;
            weightText.setFont(font);
            weightText.setCharacterSize(18);
            weightText.setFillColor(morado);
            weightText.setString(ss.str());

            Vector2f mid(
                (nodePositions[i].x + nodePositions[i + 1].x) / 2.f,
                (nodePositions[i].y + nodePositions[i + 1].y) / 2.f - 18.f
            );
            FloatRect wb = weightText.getLocalBounds();
            weightText.setOrigin(wb.left + wb.width / 2.f,
                wb.top + wb.height / 2.f);
            weightText.setPosition(mid);

            window.draw(weightText);
        }

        for (size_t i = 0; i < count; ++i) {
            const Stop* stop = lista[i];
            string name = stop ? stop->getName() : "";
            dibujarNodo(name, nodePositions[i]);
        }
        };

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Escape)) {
                window.close();
            }

            if (handleButton(event, botonGrafo, hoverGrafo)) {
                viewMode = ViewMode::Original;
                actualizarCosto("Costo total: -");
            }

            if (handleButton(event, botonPrim, hoverPrim)) {
                viewMode = ViewMode::Prim;
                double totalCost = graph.getTreePrimTotalCost();
                ostringstream ss;
                ss.setf(ios::fixed);
                ss << setprecision(2) << totalCost;
                string text = "Costo total: " + ss.str();
                actualizarCosto(text);
            }

            if (handleButton(event, botonAncho, hoverAncho)) {
                viewMode = ViewMode::BFS;
                actualizarCosto("Costo total: -");
            }

            if (handleButton(event, botonProfundo, hoverProfundo)) {
                viewMode = ViewMode::DFS;
                actualizarCosto("Costo total: -");
            }

            if (handleButton(event, botonVolver, hoverVolver)) {
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

        window.draw(boxCosto);
        window.draw(costoText);

        window.draw(areaGraph);

        if (viewMode == ViewMode::Original) {
            dibujarGrafo(graph);
        }
        else if (viewMode == ViewMode::Prim) {
            Graph* mstGraph = graph.getTreePrim();
            if (mstGraph != nullptr) {
                dibujarGrafo(*mstGraph);
                delete mstGraph;
            }
        }
        else if (viewMode == ViewMode::BFS) {
            dibujarCaminoLineal(bfsOrder);
        }
        else if (viewMode == ViewMode::DFS) {
            dibujarCaminoLineal(dfsOrder);
        }

        window.display();
    }
}

/* =====================  MODO DIJKSTRA (RUTA MÁS CORTA)  ===================== */

void WindowsGraph::runDijkstra(const Graph& graph) {
    RenderWindow window(
        mode,
        "Dijkstra",
        Style::Fullscreen
    );
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("segoeprb.ttf"))
        return;

    Color fondo(237, 237, 237);
    Color lila(194, 189, 224);
    Color morado(62, 27, 116);

    float marginTop = 30.f;
    float marginSide = 40.f;

    float fieldWidth = 260.f;
    float fieldHeight = 60.f;
    float spacing = 20.f;

    float fieldsY = marginTop + 40.f;

    TextField campoInicio;
    TextField campoFinal;

    configurarCampo(
        campoInicio,
        "Nombre parada inicio",
        font,
        Vector2f(marginSide, fieldsY),
        Vector2f(fieldWidth, fieldHeight)
    );

    configurarCampo(
        campoFinal,
        "Nombre parada Final",
        font,
        Vector2f(marginSide + fieldWidth + spacing, fieldsY),
        Vector2f(fieldWidth, fieldHeight)
    );

    float buttonWidth = 260.f;
    float buttonHeight = 60.f;

    Button botonBuscar(buttonWidth, buttonHeight, 18, font);
    botonBuscar.build(
        "Buscar ruta mas corta",
        Vector2f(marginSide + 2 * (fieldWidth + spacing), fieldsY),
        "lila"
    );

    float costoWidth = 280.f;
    float costoHeight = 60.f;

    RectangleShape boxCosto;
    boxCosto.setSize(Vector2f(costoWidth, costoHeight));
    float costoX = marginSide + 2 * (fieldWidth + spacing) + buttonWidth + spacing;
    float costoY = fieldsY;
    boxCosto.setPosition(Vector2f(costoX, costoY));
    boxCosto.setFillColor(lila);
    boxCosto.setOutlineThickness(2.f);
    boxCosto.setOutlineColor(morado);

    Text costoText;
    costoText.setFont(font);
    costoText.setCharacterSize(24);
    costoText.setFillColor(morado);
    costoText.setString("Costo total: -");
    {
        FloatRect cb = costoText.getLocalBounds();
        costoText.setOrigin(cb.left + cb.width / 2.f, cb.top + cb.height / 2.f);
        costoText.setPosition(costoX + costoWidth / 2.f, costoY + costoHeight / 2.f);
    }

    Button botonVolver(buttonWidth, buttonHeight, 20, font);
    float volverX = mode.width - marginSide - buttonWidth;
    float volverY = marginTop + 40.f;
    botonVolver.build(
        "Volver menu",
        Vector2f(volverX, volverY),
        "lila"
    );

    float areaTop = costoY + costoHeight + 40.f;

    RectangleShape areaGraph;
    areaGraph.setPosition(Vector2f(marginSide, areaTop));
    areaGraph.setSize(Vector2f(mode.width - 2.f * marginSide,
        mode.height - areaTop - 30.f));
    areaGraph.setFillColor(Color::White);
    areaGraph.setOutlineThickness(3.f);
    areaGraph.setOutlineColor(Color::Black);

    bool hoverBuscar = false;
    bool hoverVolver = false;

    vector<const Stop*> pathStops;
    bool hasPath = false;
    double currentCost = 0.0;

    auto handleButton = [&](Event& event, Button& button, bool& hoverFlag) -> bool {
        bool clicked = false;
        FloatRect bounds = button.getShape().getGlobalBounds();

        if (event.type == Event::MouseMoved) {
            Vector2i mousePos = Mouse::getPosition(window);
            bool nowHover = bounds.contains(static_cast<float>(mousePos.x),
                static_cast<float>(mousePos.y));
            hoverFlag = nowHover;

            if (nowHover)
                button.getShape().setFillColor(button.getColorHover());
            else
                button.getShape().setFillColor(button.getColorNormal());
        }

        if (event.type == Event::MouseButtonPressed &&
            event.mouseButton.button == Mouse::Left) {

            Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            if (bounds.contains(static_cast<float>(mousePos.x),
                static_cast<float>(mousePos.y))) {
                button.getShape().setFillColor(button.getColorPressed());
            }
        }

        if (event.type == Event::MouseButtonReleased &&
            event.mouseButton.button == Mouse::Left) {

            Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
            bool inside = bounds.contains(static_cast<float>(mousePos.x),
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

    auto actualizarCosto = [&]() {
        if (hasPath) {
            ostringstream ss;
            ss.setf(ios::fixed);
            ss << setprecision(2) << currentCost;
            string text = "Costo total: " + ss.str();
            costoText.setString(text);
        }
        else {
            costoText.setString("Costo total: -");
        }

        FloatRect cb = costoText.getLocalBounds();
        costoText.setOrigin(cb.left + cb.width / 2.f,
            cb.top + cb.height / 2.f);
        costoText.setPosition(costoX + costoWidth / 2.f,
            costoY + costoHeight / 2.f);
        };

    auto dibujarNodo = [&](const string& label, const Vector2f& position) {
        float nodeWidth = 150.f;
        float nodeHeight = 50.f;

        RectangleShape rect;
        rect.setSize(Vector2f(nodeWidth, nodeHeight));
        rect.setOrigin(nodeWidth / 2.f, nodeHeight / 2.f);
        rect.setPosition(position);
        rect.setFillColor(morado);

        Text text;
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(22);
        text.setFillColor(Color(194, 189, 224));

        FloatRect tb = text.getLocalBounds();
        text.setOrigin(tb.left + tb.width / 2.f,
            tb.top + tb.height / 2.f);
        text.setPosition(position);

        window.draw(rect);
        window.draw(text);
        };

    auto dibujarGrafoOriginal = [&]() {
        const vector<Stop*>& stops = graph.getStops();
        const vector<vector<pair<int, double>>>& adj = graph.getAdjacencyList();

        int n = (int)stops.size();
        if (n == 0)
            return;

        vector<Vector2f> positions;
        positions.resize(n);

        Vector2f center(
            areaGraph.getPosition().x + areaGraph.getSize().x / 2.f,
            areaGraph.getPosition().y + areaGraph.getSize().y / 2.f
        );
        float radius = min(areaGraph.getSize().x, areaGraph.getSize().y) / 3.f;

        for (int i = 0; i < n; ++i) {
            float angle = 2.f * 3.1415926f * static_cast<float>(i) / static_cast<float>(n);
            float x = center.x + radius * cos(angle);
            float y = center.y + radius * sin(angle);
            positions[i] = Vector2f(x, y);
        }

        for (int i = 0; i < n; ++i) {
            for (const auto& edge : adj[i]) {
                int j = edge.first;
                if (j <= i)
                    continue;

                Vector2f a = positions[i];
                Vector2f b = positions[j];

                Vertex line[] = {
                    Vertex(a, Color::Black),
                    Vertex(b, Color::Black)
                };
                window.draw(line, 2, Lines);

                double w = edge.second;
                ostringstream ss;
                ss.setf(ios::fixed);
                ss << setprecision(2) << w;

                Text weightText;
                weightText.setFont(font);
                weightText.setCharacterSize(18);
                weightText.setFillColor(morado);
                weightText.setString(ss.str());

                Vector2f mid(
                    (a.x + b.x) / 2.f,
                    (a.y + b.y) / 2.f - 18.f
                );
                FloatRect wb = weightText.getLocalBounds();
                weightText.setOrigin(wb.left + wb.width / 2.f,
                    wb.top + wb.height / 2.f);
                weightText.setPosition(mid);

                window.draw(weightText);
            }
        }

        for (int i = 0; i < n; ++i) {
            if (stops[i] != nullptr) {
                dibujarNodo(stops[i]->getName(), positions[i]);
            }
        }
        };

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Escape)) {
                window.close();
            }

            if (event.type == Event::MouseButtonPressed &&
                event.mouseButton.button == Mouse::Left) {

                Vector2f mousePos(
                    static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y)
                );

                bool onInicio = campoInicio.box.getGlobalBounds().contains(mousePos);
                bool onFinal = campoFinal.box.getGlobalBounds().contains(mousePos);

                campoInicio.active = onInicio;
                campoFinal.active = (!onInicio && onFinal);
            }

            if (event.type == Event::TextEntered) {
                manejarTextoCampo(campoInicio, event.text.unicode);
                manejarTextoCampo(campoFinal, event.text.unicode);
            }

            if (handleButton(event, botonBuscar, hoverBuscar)) {
                if (!campoInicio.value.empty() && !campoFinal.value.empty()) {
                    double totalCost = 0.0;
                    vector<Stop*> path = graph.getShortestPathDijkstra(
                        campoInicio.value,
                        campoFinal.value,
                        totalCost
                    );

                    pathStops.clear();
                    for (Stop* s : path) {
                        pathStops.push_back(s);
                    }

                    hasPath = !pathStops.empty();
                    currentCost = totalCost;
                    actualizarCosto();
                }
            }

            if (handleButton(event, botonVolver, hoverVolver)) {
                window.close();
            }
        }

        window.clear(fondo);

        campoInicio.box.setOutlineColor(campoInicio.active ? Color::Black : morado);
        campoFinal.box.setOutlineColor(campoFinal.active ? Color::Black : morado);

        window.draw(campoInicio.box);
        window.draw(campoInicio.label);
        window.draw(campoInicio.valueText);

        window.draw(campoFinal.box);
        window.draw(campoFinal.label);
        window.draw(campoFinal.valueText);

        window.draw(botonBuscar.getShape());
        window.draw(botonBuscar.getText());

        window.draw(boxCosto);
        window.draw(costoText);

        window.draw(botonVolver.getShape());
        window.draw(botonVolver.getText());

        window.draw(areaGraph);

        if (hasPath && pathStops.size() >= 1) {
            size_t count = pathStops.size();

            float left = areaGraph.getPosition().x + 80.f;
            float right = areaGraph.getPosition().x + areaGraph.getSize().x - 80.f;
            float y = areaGraph.getPosition().y + areaGraph.getSize().y / 2.f;

            float step = 0.f;
            if (count > 1) {
                step = (right - left) / static_cast<float>(count - 1);
            }

            vector<Vector2f> nodePositions;
            nodePositions.reserve(count);

            for (size_t i = 0; i < count; ++i) {
                float x = left + step * static_cast<float>(i);
                nodePositions.push_back(Vector2f(x, y));
            }

            for (size_t i = 0; i + 1 < count; ++i) {
                Vertex line[] = {
                    Vertex(nodePositions[i], Color::Black),
                    Vertex(nodePositions[i + 1], Color::Black)
                };
                window.draw(line, 2, Lines);

                double w = obtenerPesoArista(graph, pathStops[i], pathStops[i + 1]);
                ostringstream ss;
                ss.setf(ios::fixed);
                ss << setprecision(2) << w;

                Text weightText;
                weightText.setFont(font);
                weightText.setCharacterSize(18);
                weightText.setFillColor(morado);
                weightText.setString(ss.str());

                Vector2f mid(
                    (nodePositions[i].x + nodePositions[i + 1].x) / 2.f,
                    (nodePositions[i].y + nodePositions[i + 1].y) / 2.f - 18.f
                );
                FloatRect wb = weightText.getLocalBounds();
                weightText.setOrigin(wb.left + wb.width / 2.f,
                    wb.top + wb.height / 2.f);
                weightText.setPosition(mid);

                window.draw(weightText);
            }

            for (size_t i = 0; i < count; ++i) {
                const Stop* stop = pathStops[i];
                string name = stop ? stop->getName() : "";
                dibujarNodo(name, nodePositions[i]);
            }
        }
        else {
            dibujarGrafoOriginal();
        }

        window.display();
    }
}

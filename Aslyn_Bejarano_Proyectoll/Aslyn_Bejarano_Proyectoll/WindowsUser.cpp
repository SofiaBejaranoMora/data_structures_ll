#include "WindowsUser.h"
#include <cctype>
#include <cmath>

using namespace std;
using namespace sf;

WindowsUser::WindowsUser()
{
    mode = VideoMode::getDesktopMode();
}

void WindowsUser::configurarCampo(InputField& campo,
    const string& etiqueta,
    Vector2f position,
    Vector2f size)
{
    Color lila(194, 189, 224);
    Color morado(62, 27, 116);

    campo.box.setSize(size);
    campo.box.setPosition(position);
    campo.box.setFillColor(lila);
    campo.box.setOutlineThickness(2.f);
    campo.box.setOutlineColor(morado);

    campo.label.setFont(font);
    campo.label.setString(etiqueta);
    campo.label.setCharacterSize(22);
    campo.label.setFillColor(morado);

    FloatRect lb = campo.label.getLocalBounds();
    float centerX = position.x + size.x / 2.f;
    float labelY = position.y - 28.f;
    campo.label.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
    campo.label.setPosition(centerX, labelY);

    campo.value.clear();
    campo.valueText.setFont(font);
    campo.valueText.setCharacterSize(22);
    campo.valueText.setFillColor(morado);
    campo.valueText.setString("");

    FloatRect vb = campo.valueText.getLocalBounds();
    campo.valueText.setOrigin(vb.left, vb.top + vb.height / 2.f);
    campo.valueText.setPosition(position.x + 10.f, position.y + size.y / 2.f);

    campo.active = false;
}

void WindowsUser::manejarTextoCampo(InputField& campo,
    Uint32 unicode,
    bool soloNumeros,
    bool permitirPuntoDecimal)
{
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

    if (soloNumeros) {
        if (isdigit(static_cast<unsigned char>(c))) {
            campo.value.push_back(c);
        }
        else if (permitirPuntoDecimal && c == '.') {
            bool yaTienePunto = false;
            for (size_t i = 0; i < campo.value.size(); ++i) {
                if (campo.value[i] == '.') {
                    yaTienePunto = true;
                    break;
                }
            }
            if (!yaTienePunto)
                campo.value.push_back(c);
        }
        else {
            return;
        }
    }
    else {
        campo.value.push_back(c);
    }

    campo.valueText.setString(campo.value);
}

void WindowsUser::run(UserWindowMode modoVentana, UserTree& userTree)
{
    RenderWindow window(
        mode,
        "Usuarios",
        Style::Fullscreen
    );
    window.setFramerateLimit(60);

    if (!font.loadFromFile("segoeprb.ttf"))
        return;

    Color lila(194, 189, 224);
    Color morado(62, 27, 116);

    float margin = 60.f;
    float inputWidth = 220.f;
    float inputHeight = 60.f;
    float inputSpacing = 30.f;
    float topRowY = margin;
    float inputStartX = margin;
    float topAreaHeight = topRowY + inputHeight + 70.f;

    RectangleShape areaArbol;
    areaArbol.setPosition(margin, topAreaHeight);
    areaArbol.setSize(Vector2f(mode.width - 2.f * margin, mode.height - topAreaHeight - margin));
    areaArbol.setFillColor(Color(237, 237, 237));
    areaArbol.setOutlineThickness(3.f);
    areaArbol.setOutlineColor(Color::Black);

    float buttonWidth = 260.f;
    float buttonHeight = 60.f;
    float botonesX = mode.width - margin - buttonWidth;

    Button botonVolverMenu(buttonWidth, buttonHeight, 24, font);
    float volverY = topRowY - 20.f;
    botonVolverMenu.build("Volver menu", Vector2f(botonesX, volverY), "lila");
    botonVolverMenu.getShape().setOutlineThickness(2.f);
    botonVolverMenu.getShape().setOutlineColor(morado);
    bool hoverVolverMenu = false;

    Button botonAccion(buttonWidth, buttonHeight, 24, font);
    bool usarBotonAccion = false;
    bool hoverAccion = false;

    Text tituloVentana;
    tituloVentana.setFont(font);
    tituloVentana.setCharacterSize(30);
    tituloVentana.setFillColor(morado);

    bool usarCedula = false;
    bool usarNombre = false;
    bool usarEdad = false;
    bool usarSaldo = false;

    if (modoVentana == UserWindowMode::Registrar) {
        configurarCampo(campoCedula, "Cedula",
            Vector2f(inputStartX + 0 * (inputWidth + inputSpacing), topRowY),
            Vector2f(inputWidth, inputHeight));

        configurarCampo(campoNombre, "Nombre",
            Vector2f(inputStartX + 1 * (inputWidth + inputSpacing), topRowY),
            Vector2f(inputWidth, inputHeight));

        configurarCampo(campoEdad, "Edad",
            Vector2f(inputStartX + 2 * (inputWidth + inputSpacing), topRowY),
            Vector2f(inputWidth, inputHeight));

        configurarCampo(campoSaldo, "Saldo de cuenta",
            Vector2f(inputStartX + 3 * (inputWidth + inputSpacing), topRowY),
            Vector2f(inputWidth, inputHeight));

        float registerY = volverY + 70.f;
        botonAccion.build("Registrar usuario",
            Vector2f(botonesX, registerY),
            "lila");
        botonAccion.getShape().setOutlineThickness(2.f);
        botonAccion.getShape().setOutlineColor(morado);

        usarBotonAccion = true;
        usarCedula = usarNombre = usarEdad = usarSaldo = true;
        tituloVentana.setString("");
    }
    else if (modoVentana == UserWindowMode::Eliminar) {
        configurarCampo(campoCedula, "Cedula",
            Vector2f(inputStartX, topRowY),
            Vector2f(inputWidth, inputHeight));

        float eliminarY = volverY + 70.f;
        botonAccion.build("Eliminar",
            Vector2f(botonesX, eliminarY),
            "lila");
        botonAccion.getShape().setOutlineThickness(2.f);
        botonAccion.getShape().setOutlineColor(morado);

        usarBotonAccion = true;
        usarCedula = true;
        tituloVentana.setString("");
    }
    else if (modoVentana == UserWindowMode::Consultar) {
        tituloVentana.setString("Consultar Usuarios");
        FloatRect tb = tituloVentana.getLocalBounds();
        tituloVentana.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
        tituloVentana.setPosition(mode.width / 2.f, topRowY + inputHeight / 2.f);
        usarBotonAccion = false;
    }

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

                Vector2f mousePos(static_cast<float>(event.mouseButton.x),
                    static_cast<float>(event.mouseButton.y));

                if (usarCedula)
                    campoCedula.active = campoCedula.box.getGlobalBounds().contains(mousePos);
                else
                    campoCedula.active = false;

                if (usarNombre)
                    campoNombre.active = campoNombre.box.getGlobalBounds().contains(mousePos);
                else
                    campoNombre.active = false;

                if (usarEdad)
                    campoEdad.active = campoEdad.box.getGlobalBounds().contains(mousePos);
                else
                    campoEdad.active = false;

                if (usarSaldo)
                    campoSaldo.active = campoSaldo.box.getGlobalBounds().contains(mousePos);
                else
                    campoSaldo.active = false;
            }

            if (event.type == Event::TextEntered) {
                if (modoVentana == UserWindowMode::Registrar ||
                    modoVentana == UserWindowMode::Eliminar) {

                    if (!(campoCedula.active &&
                        campoCedula.value.size() >= 9 &&
                        event.text.unicode != 8)) {
                        manejarTextoCampo(campoCedula, event.text.unicode, true, false);
                    }

                    manejarTextoCampo(campoNombre, event.text.unicode, false, false);

                    if (!(campoEdad.active &&
                        campoEdad.value.size() >= 3 &&
                        event.text.unicode != 8)) {
                        manejarTextoCampo(campoEdad, event.text.unicode, true, false);
                    }

                    manejarTextoCampo(campoSaldo, event.text.unicode, true, true);
                }
            }

            auto handleButton = [&](Button& button, bool& hoverFlag) -> bool {
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

            if (handleButton(botonVolverMenu, hoverVolverMenu)) {
                window.close();
            }

            if (usarBotonAccion) {
                if (handleButton(botonAccion, hoverAccion)) {
                    if (modoVentana == UserWindowMode::Registrar) {
                        if (campoCedula.value.size() == 9 &&
                            !campoNombre.value.empty() &&
                            campoEdad.value.size() >= 1 && campoEdad.value.size() <= 3 &&
                            !campoSaldo.value.empty()) {

                            int id = stoi(campoCedula.value);
                            string nombre = campoNombre.value;
                            int edad = stoi(campoEdad.value);
                            double saldo = stod(campoSaldo.value);

                            User nuevo(id, nombre, edad, saldo);
                            userTree.insertUser(nuevo);

                            campoCedula.value.clear();
                            campoNombre.value.clear();
                            campoEdad.value.clear();
                            campoSaldo.value.clear();

                            campoCedula.valueText.setString("");
                            campoNombre.valueText.setString("");
                            campoEdad.valueText.setString("");
                            campoSaldo.valueText.setString("");
                        }
                    }
                    else if (modoVentana == UserWindowMode::Eliminar) {
                        if (campoCedula.value.size() == 9) {
                            int id = stoi(campoCedula.value);
                            userTree.removeUser(id);
                            campoCedula.value.clear();
                            campoCedula.valueText.setString("");
                        }
                    }
                }
            }
        }

        window.clear(lila);

        if (usarCedula)
            campoCedula.box.setOutlineColor(campoCedula.active ? Color::Black : morado);
        if (usarNombre)
            campoNombre.box.setOutlineColor(campoNombre.active ? Color::Black : morado);
        if (usarEdad)
            campoEdad.box.setOutlineColor(campoEdad.active ? Color::Black : morado);
        if (usarSaldo)
            campoSaldo.box.setOutlineColor(campoSaldo.active ? Color::Black : morado);

        window.draw(areaArbol);

        if (modoVentana == UserWindowMode::Registrar) {
            window.draw(campoCedula.box);
            window.draw(campoCedula.label);
            window.draw(campoCedula.valueText);

            window.draw(campoNombre.box);
            window.draw(campoNombre.label);
            window.draw(campoNombre.valueText);

            window.draw(campoEdad.box);
            window.draw(campoEdad.label);
            window.draw(campoEdad.valueText);

            window.draw(campoSaldo.box);
            window.draw(campoSaldo.label);
            window.draw(campoSaldo.valueText);
        }
        else if (modoVentana == UserWindowMode::Eliminar) {
            window.draw(campoCedula.box);
            window.draw(campoCedula.label);
            window.draw(campoCedula.valueText);
        }
        else if (modoVentana == UserWindowMode::Consultar) {
            window.draw(tituloVentana);
        }

        window.draw(botonVolverMenu.getShape());
        window.draw(botonVolverMenu.getText());

        if (usarBotonAccion) {
            window.draw(botonAccion.getShape());
            window.draw(botonAccion.getText());
        }

        NodeTree<User>* root = userTree.getRoot();
        if (root != nullptr) {
            float centerX = areaArbol.getPosition().x + areaArbol.getSize().x / 2.f;
            float startYTree = areaArbol.getPosition().y + 60.f;
            float offsetX = areaArbol.getSize().x / 4.f;
            float levelStep = 90.f;
            drawTree(window, root, Vector2f(centerX, startYTree), offsetX, levelStep);
        }

        window.display();
    }
}

void WindowsUser::drawEdge(RenderWindow& window,
    Vector2f start,
    Vector2f end)
{
    float thickness = 3.f;
    Color color = Color::Black;

    Vector2f diff = end - start;
    float length = sqrt(diff.x * diff.x + diff.y * diff.y);
    float angle = atan2(diff.y, diff.x) * 180.f / 3.14159265f;

    RectangleShape line;
    line.setSize(Vector2f(length, thickness));
    line.setFillColor(color);
    line.setOrigin(0.f, thickness / 2.f);
    line.setPosition(start);
    line.setRotation(angle);

    window.draw(line);
}

void WindowsUser::drawTree(RenderWindow& window,
    NodeTree<User>* node,
    Vector2f position,
    float offsetX,
    float levelStep)
{
    if (node == nullptr)
        return;

    string label = node->getData().getName();
    drawNode(window, label, position);

    NodeTree<User>* left = node->getLeft();
    NodeTree<User>* right = node->getRight();

    float halfNodeHeight = 20.f;

    if (left != nullptr) {
        Vector2f childCenter(position.x - offsetX, position.y + levelStep);
        Vector2f start(position.x, position.y + halfNodeHeight);
        Vector2f end(childCenter.x, childCenter.y - halfNodeHeight);
        drawEdge(window, start, end);
        drawTree(window, left, childCenter, offsetX / 2.f, levelStep);
    }

    if (right != nullptr) {
        Vector2f childCenter(position.x + offsetX, position.y + levelStep);
        Vector2f start(position.x, position.y + halfNodeHeight);
        Vector2f end(childCenter.x, childCenter.y - halfNodeHeight);
        drawEdge(window, start, end);
        drawTree(window, right, childCenter, offsetX / 2.f, levelStep);
    }
}

void WindowsUser::drawNode(RenderWindow& window,
    const string& label,
    Vector2f position)
{
    float nodeWidth = 120.f;
    float nodeHeight = 40.f;

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
    text.setCharacterSize(20);
    text.setFillColor(lila);

    FloatRect tb = text.getLocalBounds();
    text.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    text.setPosition(position);

    window.draw(text);
}

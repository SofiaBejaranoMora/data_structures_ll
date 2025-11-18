#include "WindowsOrder.h"

using namespace std;
using namespace sf;

WindowsOrder::WindowsOrder()
{
    mode = VideoMode::getDesktopMode();
}

void WindowsOrder::actualizarTextoDesdeLista(const list<User>& usuarios)
{
    string contenido;
    bool first = true;

    for (const User& u : usuarios) {
        if (!first) {
            contenido += "\n\n";
        }
        first = false;
        contenido += u.toString();
    }

    textoResultados.setString(contenido);
}

void WindowsOrder::run(UserTree& userTree)
{
    RenderWindow window(
        mode,
        "Ordenes del arbol",
        Style::Fullscreen
    );
    window.setFramerateLimit(60);

    if (!font.loadFromFile("segoeprb.ttf"))
        return;

    Color grisFondo(0xED, 0xED, 0xED);
    Color morado(62, 27, 116);
    Color lila(194, 189, 224);

    float margin = 60.f;

    float buttonWidth = 260.f;
    float buttonHeight = 60.f;
    float buttonSpacing = 40.f;
    float botonesY = margin;

    int numButtons = 4;
    float totalButtonsWidth =
        numButtons * buttonWidth + (numButtons - 1) * buttonSpacing;
    float startX = (mode.width - totalButtonsWidth) / 2.f;

    Button botonPreOrden(buttonWidth, buttonHeight, 24, font);
    botonPreOrden.build("PreOrden", Vector2f(startX, botonesY), "lila");
    botonPreOrden.getShape().setOutlineThickness(2.f);
    botonPreOrden.getShape().setOutlineColor(morado);

    Button botonPosOrden(buttonWidth, buttonHeight, 24, font);
    botonPosOrden.build("PosOrden",
        Vector2f(startX + (buttonWidth + buttonSpacing), botonesY), "lila");
    botonPosOrden.getShape().setOutlineThickness(2.f);
    botonPosOrden.getShape().setOutlineColor(morado);

    Button botonEntreOrden(buttonWidth, buttonHeight, 24, font);
    botonEntreOrden.build("EntreOrden",
        Vector2f(startX + 2 * (buttonWidth + buttonSpacing), botonesY), "lila");
    botonEntreOrden.getShape().setOutlineThickness(2.f);
    botonEntreOrden.getShape().setOutlineColor(morado);

    Button botonVolver(buttonWidth, buttonHeight, 24, font);
    botonVolver.build("Volver menu",
        Vector2f(startX + 3 * (buttonWidth + buttonSpacing), botonesY), "lila");
    botonVolver.getShape().setOutlineThickness(2.f);
    botonVolver.getShape().setOutlineColor(morado);

    bool hoverPre = false;
    bool hoverPos = false;
    bool hoverEntre = false;
    bool hoverVolver = false;

    float areaY = botonesY + buttonHeight + 60.f;
    areaResultados.setPosition(margin, areaY);
    areaResultados.setSize(
        Vector2f(mode.width - 2.f * margin, mode.height - areaY - margin));
    areaResultados.setFillColor(Color(0x3E, 0x1B, 0x74));
    areaResultados.setOutlineThickness(4.f);
    areaResultados.setOutlineColor(Color::Black);

    textoResultados.setFont(font);
    textoResultados.setCharacterSize(28);
    textoResultados.setFillColor(lila);
    textoResultados.setPosition(
        areaResultados.getPosition().x + 30.f,
        areaResultados.getPosition().y + 30.f
    );
    textoResultados.setLineSpacing(1.3f);

    list<User> usuariosInicial = userTree.getInOrder();
    actualizarTextoDesdeLista(usuariosInicial);

    FloatRect areaBounds = areaResultados.getGlobalBounds();

    View defaultView = window.getDefaultView();
    View viewResultados;

    viewResultados.setSize(areaBounds.width, areaBounds.height);
    viewResultados.setCenter(
        areaBounds.left + areaBounds.width / 2.f,
        areaBounds.top + areaBounds.height / 2.f
    );
    viewResultados.setViewport(FloatRect(
        areaBounds.left / mode.width,
        areaBounds.top / mode.height,
        areaBounds.width / mode.width,
        areaBounds.height / mode.height
    ));

    float scrollOffset = 0.f;
    float maxScroll = 0.f;

    auto recomputeMaxScroll = [&]() {
        FloatRect tb = textoResultados.getLocalBounds();
        float contentHeight = tb.height + 60.f;
        float viewHeight = areaBounds.height;
        maxScroll = (contentHeight > viewHeight)
            ? (contentHeight - viewHeight)
            : 0.f;

        if (scrollOffset > maxScroll) scrollOffset = maxScroll;
        if (scrollOffset < 0.f)       scrollOffset = 0.f;
        };

    recomputeMaxScroll();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Escape)) {
                window.close();
            }

            if (event.type == Event::MouseWheelScrolled &&
                event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {

                Vector2f mouseWorld(
                    static_cast<float>(event.mouseWheelScroll.x),
                    static_cast<float>(event.mouseWheelScroll.y));

                if (areaBounds.contains(mouseWorld)) {
                    float delta = event.mouseWheelScroll.delta;
                    scrollOffset -= delta * 40.f;
                    if (scrollOffset < 0.f) scrollOffset = 0.f;
                    if (scrollOffset > maxScroll) scrollOffset = maxScroll;
                }
            }

            auto handleButton = [&](Button& button, bool& hoverFlag) -> bool {
                bool clicked = false;
                FloatRect bounds = button.getShape().getGlobalBounds();

                if (event.type == Event::MouseMoved) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    bool nowHover = bounds.contains(
                        static_cast<float>(mousePos.x),
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
                    if (bounds.contains(
                        static_cast<float>(mousePos.x),
                        static_cast<float>(mousePos.y))) {
                        button.getShape().setFillColor(button.getColorPressed());
                    }
                }

                if (event.type == Event::MouseButtonReleased &&
                    event.mouseButton.button == Mouse::Left) {
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

            if (handleButton(botonPreOrden, hoverPre)) {
                list<User> usuarios = userTree.getPreOrder();
                actualizarTextoDesdeLista(usuarios);
                scrollOffset = 0.f;
                recomputeMaxScroll();
            }

            if (handleButton(botonPosOrden, hoverPos)) {
                list<User> usuarios = userTree.getPostOrder();
                actualizarTextoDesdeLista(usuarios);
                scrollOffset = 0.f;
                recomputeMaxScroll();
            }

            if (handleButton(botonEntreOrden, hoverEntre)) {
                list<User> usuarios = userTree.getInOrder();
                actualizarTextoDesdeLista(usuarios);
                scrollOffset = 0.f;
                recomputeMaxScroll();
            }

            if (handleButton(botonVolver, hoverVolver)) {
                window.close();
            }
        }

        float centerX = areaBounds.left + areaBounds.width / 2.f;
        float baseCenterY = areaBounds.top + areaBounds.height / 2.f;
        viewResultados.setCenter(centerX, baseCenterY + scrollOffset);

        window.clear(grisFondo);

        window.setView(defaultView);

        window.draw(botonPreOrden.getShape());
        window.draw(botonPreOrden.getText());

        window.draw(botonPosOrden.getShape());
        window.draw(botonPosOrden.getText());

        window.draw(botonEntreOrden.getShape());
        window.draw(botonEntreOrden.getText());

        window.draw(botonVolver.getShape());
        window.draw(botonVolver.getText());

        window.draw(areaResultados);

        window.setView(viewResultados);
        window.draw(textoResultados);

        window.setView(defaultView);
        window.display();
    }
}

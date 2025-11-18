#include "windowsMenu.h"
#include "Button.h"

using namespace sf;

windowsMenu::windowsMenu()
{
    mode = VideoMode::getDesktopMode();
}

void windowsMenu::run()
{
    RenderWindow window(
        mode,
        "Rutas",
        Style::Fullscreen
    );
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("segoeprb.ttf"))
        return;

    float buttonWidth = 300.f;
    float buttonHeight = 70.f;
    float spacing = 20.f;
    int numButtons = 4;

    float centerX = mode.width / 2.f;
    float totalHeight = numButtons * buttonHeight + (numButtons - 1) * spacing;
    float startY = (mode.height - totalHeight) / 2.f;
    float xPos = centerX - buttonWidth / 2.f;

    Button botonRegistrarUsuario(buttonWidth, buttonHeight, 28, font);
    botonRegistrarUsuario.build("Registrar Usuario",
        Vector2f(xPos, startY + 0 * (buttonHeight + spacing)), "morado");

    Button botonEliminarUsuario(buttonWidth, buttonHeight, 28, font);
    botonEliminarUsuario.build("Eliminar Usuario",
        Vector2f(xPos, startY + 1 * (buttonHeight + spacing)), "morado");

    Button botonConsultarUsuarios(buttonWidth, buttonHeight, 28, font);
    botonConsultarUsuarios.build("Consultar Usuarios",
        Vector2f(xPos, startY + 2 * (buttonHeight + spacing)), "morado");

    Button botonSalirSistema(buttonWidth, buttonHeight, 28, font);
    botonSalirSistema.build("Salir",
        Vector2f(xPos, startY + 3 * (buttonHeight + spacing)), "morado");

    bool hoverRegistrarUsuario = false;
    bool hoverEliminarUsuario = false;
    bool hoverConsultarUsuarios = false;
    bool hoverSalirSistema = false;

    WindowsUser ventanaUsuarios;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Escape)) {
                window.close();
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

            if (handleButton(botonRegistrarUsuario, hoverRegistrarUsuario)) {
                window.setVisible(false);
                ventanaUsuarios.run(UserWindowMode::Registrar, userTree);
                window.setVisible(true);
            }

            if (handleButton(botonEliminarUsuario, hoverEliminarUsuario)) {
                window.setVisible(false);
                ventanaUsuarios.run(UserWindowMode::Eliminar, userTree);
                window.setVisible(true);
            }

            if (handleButton(botonConsultarUsuarios, hoverConsultarUsuarios)) {
                window.setVisible(false);
                ventanaUsuarios.run(UserWindowMode::Consultar, userTree);
                window.setVisible(true);
            }

            if (handleButton(botonSalirSistema, hoverSalirSistema)) {
                window.close();
            }
        }

        window.clear(Color(194, 189, 224));

        window.draw(botonRegistrarUsuario.getShape());
        window.draw(botonRegistrarUsuario.getText());

        window.draw(botonEliminarUsuario.getShape());
        window.draw(botonEliminarUsuario.getText());

        window.draw(botonConsultarUsuarios.getShape());
        window.draw(botonConsultarUsuarios.getText());

        window.draw(botonSalirSistema.getShape());
        window.draw(botonSalirSistema.getText());

        window.display();
    }
}

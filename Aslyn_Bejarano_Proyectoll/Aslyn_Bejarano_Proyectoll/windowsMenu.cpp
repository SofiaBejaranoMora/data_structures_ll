#include "windowsMenu.h"
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "windowsMenu.h"

windowsMenu::windowsMenu()
{
    VideoMode mode = VideoMode::getDesktopMode();

    RenderWindow window(
        mode,
        "Rutas",
        Style::Fullscreen
    );
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("segoeprb.ttf"))
        return;

    Button exitButton(250.f, 70.f, 28, font);
    exitButton.build("Salir", Vector2f(50.f, 50.f), "morado");

    bool hoverExit = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Escape)) {
                window.close();
            }

            FloatRect bounds = exitButton.getShape().getGlobalBounds();

            if (event.type == Event::MouseMoved) {
                Vector2i mousePos = Mouse::getPosition(window);
                bool nowHover = bounds.contains(static_cast<float>(mousePos.x),
                    static_cast<float>(mousePos.y));
                hoverExit = nowHover;

                if (nowHover)
                    exitButton.getShape().setFillColor(exitButton.getColorHover());
                else
                    exitButton.getShape().setFillColor(exitButton.getColorNormal());
            }

            if (event.type == Event::MouseButtonPressed &&
                event.mouseButton.button == Mouse::Left) {

                Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
                if (bounds.contains(static_cast<float>(mousePos.x),
                    static_cast<float>(mousePos.y))) {
                    exitButton.getShape().setFillColor(exitButton.getColorPressed());
                }
            }

            if (event.type == Event::MouseButtonReleased &&
                event.mouseButton.button == Mouse::Left) {

                Vector2i mousePos(event.mouseButton.x, event.mouseButton.y);
                bool inside = bounds.contains(static_cast<float>(mousePos.x),
                    static_cast<float>(mousePos.y));

                if (inside)
                    window.close();

                exitButton.getShape().setFillColor(
                    hoverExit ? exitButton.getColorHover()
                    : exitButton.getColorNormal()
                );
            }
        }

        window.clear(Color(0xED, 0xED, 0xED));

        window.draw(exitButton.getShape());
        window.draw(exitButton.getText());

        window.display();
}

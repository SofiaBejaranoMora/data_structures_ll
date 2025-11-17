#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class Button {
private:
    float width;
    float height;
    unsigned int fontSize;
    const Font& font;

    RectangleShape shape;
    Text text;

    Color colorNormal;
    Color colorHover;
    Color colorPressed;
    Color textColor;

    void configureColors(const std::string& colorName);
    void centerText();

public:
    Button(float width, float height, unsigned int fontSize, const Font& font);

    void build(const std::string& label,
        const Vector2f& position,
        const std::string& colorName);

    RectangleShape& getShape();
    Text& getText();
    Color getColorNormal() const;
    Color getColorHover() const;
    Color getColorPressed() const;
};

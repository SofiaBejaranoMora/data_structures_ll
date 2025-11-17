#include "Button.h"

Button::Button(float width, float height, unsigned int fontSize, const Font& font)
    : width(width), height(height), fontSize(fontSize), font(font)
{
}

void Button::configureColors(const std::string& colorName)
{
    if (colorName == "morado") {
        colorNormal = Color(62, 27, 116);
        colorHover = Color(82, 47, 136);
        colorPressed = Color(42, 7, 96);
        textColor = Color(194, 189, 224);
    }
    else if (colorName == "lila") {
        colorNormal = Color(194, 189, 224);
        colorHover = Color(214, 209, 244);
        colorPressed = Color(174, 169, 204);
        textColor = Color(62, 27, 116);
    }
    else if (colorName == "gris") {
        colorNormal = Color(217, 217, 217);
        colorHover = Color(237, 237, 237);
        colorPressed = Color(197, 197, 197);
        textColor = Color::Black;
    }
    else {
        colorNormal = Color(217, 217, 217);
        colorHover = Color(237, 237, 237);
        colorPressed = Color(197, 197, 197);
        textColor = Color::Black;
    }

    shape.setFillColor(colorNormal);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(Color(50, 50, 50));
}

void Button::centerText()
{
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin(bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f);

    Vector2f pos = shape.getPosition();
    Vector2f size = shape.getSize();

    text.setPosition(pos.x + size.x / 2.f,
        pos.y + size.y / 2.f);
}

void Button::build(const std::string& label,
    const Vector2f& position,
    const std::string& colorName)
{
    shape.setSize(Vector2f(width, height));
    shape.setPosition(position);

    configureColors(colorName);

    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(fontSize);
    text.setFillColor(textColor);

    centerText();
}

RectangleShape& Button::getShape()
{
    return shape;
}

Text& Button::getText()
{
    return text;
}

Color Button::getColorNormal() const
{
    return colorNormal;
}

Color Button::getColorHover() const
{
    return colorHover;
}

Color Button::getColorPressed() const
{
    return colorPressed;
}

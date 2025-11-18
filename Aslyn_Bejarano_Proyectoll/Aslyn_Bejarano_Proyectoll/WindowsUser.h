#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "UserTree.h"
#include "User.h"
#include "NodeTree.h"
#include "Button.h"

using namespace std;
using namespace sf;

enum class UserWindowMode {
    Registrar,
    Eliminar,
    Consultar
};

struct InputField {
    RectangleShape box;
    Text label;
    Text valueText;
    string value;
    bool active;
};

class WindowsUser {
private:
    VideoMode mode;
    Font font;

    InputField campoCedula;
    InputField campoNombre;
    InputField campoEdad;
    InputField campoSaldo;

    void configurarCampo(InputField& campo,
        const string& etiqueta,
        Vector2f position,
        Vector2f size);

    void manejarTextoCampo(InputField& campo,
        Uint32 unicode,
        bool soloNumeros,
        bool permitirPuntoDecimal);

    void drawEdge(RenderWindow& window,
        Vector2f start,
        Vector2f end);

    void drawTree(RenderWindow& window,
        NodeTree<User>* node,
        Vector2f position,
        float offsetX,
        float levelStep);

    void drawNode(RenderWindow& window,
        const string& label,
        Vector2f position);

public:
    WindowsUser();

    void run(UserWindowMode modoVentana, UserTree& userTree);
};

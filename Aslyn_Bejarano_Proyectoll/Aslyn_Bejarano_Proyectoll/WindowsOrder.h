#pragma once

#include <SFML/Graphics.hpp>
#include "UserTree.h"
#include "Button.h"
#include <list>
#include <string>

using namespace std;
using namespace sf;

class WindowsOrder {
private:
    VideoMode mode;
    Font font;

    RectangleShape areaResultados;
    Text textoResultados;

    void actualizarTextoDesdeLista(const list<User>& usuarios);

public:
    WindowsOrder();
    void run(UserTree& userTree);
};

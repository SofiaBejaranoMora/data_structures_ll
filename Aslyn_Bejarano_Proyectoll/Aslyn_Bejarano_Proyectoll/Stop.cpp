#include "Stop.h"

Stop::Stop() {
    id = 0;
    name = "";
    description = "";
}

Stop::Stop(int id, const string& name, const string& description) {
    this->id = id;
    this->name = name;
    this->description = description;
}

int Stop::getId() const {
    return id;
}

string Stop::getName() const {
    return name;
}

string Stop::getDescription() const {
    return description;
}

void Stop::setId(int id) {
    this->id = id;
}

void Stop::setName(const string& name) {
    this->name = name;
}

void Stop::setDescription(const string& description) {
    this->description = description;
}

string Stop::toString() const {
    return string("Codigo parada: ") + to_string(id) + "\n" +
        "Nombre: " + name + "\n" +
        "Descripcion: " + description;
}

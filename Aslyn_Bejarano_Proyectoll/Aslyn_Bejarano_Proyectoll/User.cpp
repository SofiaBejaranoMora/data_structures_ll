#include "User.h"

using namespace std;

User::User() {
    id = 0;
    name = "";
    age = 0;
    balance = 0.0;
}

User::User(int id, const string& name, int age, double balance) {
    this->id = id;
    this->name = name;
    this->age = age;
    this->balance = balance;
}

int User::getId() const {
    return id;
}

string User::getName() const {
    return name;
}

int User::getAge() const {
    return age;
}

double User::getBalance() const {
    return balance;
}

void User::setId(int id) {
    this->id = id;
}

void User::setName(const string& name) {
    this->name = name;
}

void User::setAge(int age) {
    this->age = age;
}

void User::setBalance(double balance) {
    this->balance = balance;
}

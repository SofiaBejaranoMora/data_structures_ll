#pragma once
#include <string>

using namespace std;

class User {
private:
    int id;
    string name;
    int age;
    double balance;

public:
    User();
    User(int id, const string& name, int age, double balance);

    int getId() const;
    string getName() const;
    int getAge() const;
    double getBalance() const;

    void setId(int id);
    void setName(const string& name);
    void setAge(int age);
    void setBalance(double balance);
};

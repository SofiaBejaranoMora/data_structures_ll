#pragma once
#include <string>

using namespace std;

class Stop {
private:
    int id;
    string name;
    string description;

public:
    Stop();
    Stop(int id, const string& name, const string& description);

    int getId() const;
    string getName() const;
    string getDescription() const;

    void setId(int id);
    void setName(const string& name);
    void setDescription(const string& description);

    string toString() const;
};

#pragma once

#include "event.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class Frame
{
private:
public:
    Frame();

    ~Frame();
    Frame(const Frame &other);
    Frame(&operator=(const Frame &other));
    Frame(Frame &&other);
    Frame &operator=(Frame &&other);

    string connect(string username, string password, string host, string acceptVersion);
    string disconnect(int receipt);
    string send(string username, string team_a_name, string team_b_name, Event &event);
    string subscribe(string destination, int id, int receipt);
    string unsubscribe(int id, int receipt);
    string summary(string team_a_name, string team_b_name, vector<Event> &events);
};

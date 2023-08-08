#pragma once

#include "event.h"

#include <map>
#include <string>
#include <vector>
#include <vector>

using std::map;
using std::string;
using std::pair;
using std::vector;

class Reports
{
private:
    map<pair<string,string>,vector<Event>> mUsers_and_channels_to_reports;
public:
    Reports();
    virtual ~Reports();
    Reports(const Reports &other);
    Reports(&operator=(const Reports &other));
    Reports(Reports &&other);
    Reports &operator=(Reports &&other);

    void addReport(pair<string,string> &user_and_channel, Event &report);
    bool getReports(pair<string,string> &user_and_channel, vector<Event> *events);
};


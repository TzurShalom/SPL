#include "Reports.h"

Reports::Reports() : mUsers_and_channels_to_reports()
{
    map<pair<string,string>,vector<Event>> mUsers_and_channels_to_reports;
}

Reports::~Reports()
{
    mUsers_and_channels_to_reports.clear();
}

void Reports::addReport(pair<string,string> &user_and_channel, Event &report)
{
    
    if (mUsers_and_channels_to_reports.count(user_and_channel)) 
    {
        mUsers_and_channels_to_reports[user_and_channel].push_back(report);
    }
    else
    {
        vector<Event> reports; 
        reports.push_back(report);
        mUsers_and_channels_to_reports[user_and_channel] = reports;
    }
}

bool Reports::getReports(pair<string,string> &user_and_channel, vector<Event> *events)
{
    if (mUsers_and_channels_to_reports.count(user_and_channel))
    {
        *events = mUsers_and_channels_to_reports[user_and_channel];
        return true;
    }
    return false;
}


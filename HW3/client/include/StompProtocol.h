#pragma once

#include "ConnectionHandler.h"
#include "event.h"
#include "Reports.h"
#include "Client.h"

class StompProtocol
{
    private:
        ConnectionHandler *mConnectionHandler;
        Client *mClient;
        Reports *mReports;
    public:
        StompProtocol(ConnectionHandler *connectionHandler, Client *client, Reports *reports);

        ~StompProtocol();
        StompProtocol(const StompProtocol &other);
        StompProtocol(&operator=(const StompProtocol &other));
        StompProtocol(StompProtocol &&other);
        StompProtocol &operator=(StompProtocol &&other);

        map<string,string> convertStringToMap(vector<string> &lines);
        Event convertStringToEvent(string &message);
        void process();
};
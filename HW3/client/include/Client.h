#pragma once

#include <string>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::pair;
using std::map;

class Client
{
private:
    vector<pair<string,string>> mReceipts;
    map<string,int> mChannelToId;
    string mUsername;
    bool loggedIn;
    bool wait;
    int receiptCounter;
    int idCounter;

public:
    Client(string username);

    virtual ~Client();
    Client(const Client &other);
    Client(&operator=(const Client &other));
    Client(Client &&other);
    Client &operator=(Client &&other);

    string getUsername() const;
    void setUsername(string name);

    bool getState() const;
    void setState(bool state);

    bool getWait() const;
    void setWait(bool state);

    int getReceiptCounter() const;
    void raiseByOneReceiptCounter();

    int getIdCounter() const;
    void raiseByOneIdCounter();
    
    string getReceipt(string receipt) const;
    void addReceipt(string receipt, string command);

    int getIdByChannel(string channel);
    void addIdAndChannel(string channel, int id);
    void removeIdByChannel(string channel);
};

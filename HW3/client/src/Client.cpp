#include "Client.h"

Client::Client(string username) : mReceipts(), mChannelToId(), mUsername(username),
loggedIn(false), wait(true), receiptCounter(1), idCounter(1)
{
    vector<pair<string,string>> receipts;
    mReceipts = receipts;

    map<string,int> channelToId;
    mChannelToId = channelToId;
}

Client::~Client()
{
    mReceipts.clear();
    mChannelToId.clear();
}

string Client::getUsername() const {return mUsername;}
void Client::setUsername(string name) {mUsername = name;}

bool Client::getState() const {return loggedIn;}
void Client::setState(bool state) {loggedIn = state;}

bool Client::getWait() const {return wait;}
void Client::setWait(bool state) {wait = state;}

int Client::getReceiptCounter() const {return receiptCounter;}
void Client::raiseByOneReceiptCounter() {receiptCounter++;}

int Client::getIdCounter() const {return idCounter;}
void Client::raiseByOneIdCounter() {idCounter++;}

string Client::getReceipt(string receipt) const
{
    for (pair<string,string> p : mReceipts)
    {
        if (p.first == receipt)
        {
            return p.second;
        }
    }
    return "empty-string";
}

void Client::addReceipt(string receipt, string command)
{
    pair<string,string> p;
    p.first = receipt;
    p.second = command;
    mReceipts.push_back(p);
}

int Client::getIdByChannel(string channel)
{
    if (mChannelToId.count(channel))
    {
        return mChannelToId[channel];
    }
    return -1;
} 

void Client::addIdAndChannel(string channel, int id) {mChannelToId.insert({channel,id});}
void Client::removeIdByChannel(string channel) {mChannelToId.erase(channel);}

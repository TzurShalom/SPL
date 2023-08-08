#include "StompProtocol.h"
#include <boost/algorithm/string.hpp>
#include <string>

using namespace std;
using boost::split;
using boost::is_any_of;
using boost::replace_all;

StompProtocol::~StompProtocol()
{
    //  if (mConnectionHandler) {mConnectionHandler = nullptr;}
    //  if (mClient) {mClient = nullptr;}
    //  if (mReports) {mReports = nullptr;}
}

StompProtocol::StompProtocol(ConnectionHandler *connectionHandler, Client *client, Reports *reports) :
    mConnectionHandler(connectionHandler), mClient(client), mReports(reports){}

map<string,string> StompProtocol::convertStringToMap(vector<string> &lines)
{
    map<string,string> map;

    int i = 0;
    map["stomp-command"] = lines[i];
    i++;

    vector<string> header;
    string key; 
    string value;

    while(true)
    {
        if (lines[i] == "") {break;}
        split(header,lines[i],is_any_of(":"));
        key = header[0];
        value = header[1];
        map[key] = value;
        i++;
    }

    if ((unsigned) i < lines.size())
    {
        i++;
        string message = "";

        while((unsigned) i < lines.size())
        {
            message = message +  lines[i] + "\n";
            i++;
        }
        map["message"] = message;
    }
    return map;
}

Event StompProtocol::convertStringToEvent(string &message) // text -> map -> event
{
        map<string,string> m;
        map<string,string> general_game_updates;
        map<string,string> team_a_updates;
        map<string,string> team_b_updates;

        vector<string> lines;
        split(lines,message,is_any_of("\n"));

        vector<string> update;
        string key; 
        string value;
        string tab = "    ";
        int i = 1;

        while((unsigned) i < lines.size() - 1)
        {
            split(update,lines[i],is_any_of(":"));

            key = update[0]; value = update[1];
                
            if(key == "general game updates")
            {
                i++;
                while (lines[i].substr(0,4) == tab)
                {
                    split(update,lines[i],is_any_of(":"));
                    key = update[0]; value = update[1];
                    general_game_updates[key] = value;
                    i++;
                }
            }
            else if (key == "team a updates")
            {
                i++;
                while (lines[i].substr(0,4) == tab)
                {
                    split(update,lines[i],is_any_of(":"));
                    key = update[0]; value = update[1];
                    team_a_updates[key] = value;
                    i++;
                }
            }
            else if (key == "team b updates")
            {
                i++;
                while (lines[i].substr(0,4) == tab)
                {
                    split(update,lines[i],is_any_of(":"));
                    key = update[0]; value = update[1];
                    team_b_updates[key] = value;
                    i++;
                }
            }
            else if (key == "description") {i++; m[key] = lines[i]; i++;}
            else {m[key] = value; i++;}
        }

        return Event(m["team a"]+"_"+m["team b"],
        m["team a"],
        m["team b"],
        stoi(m["time"]),
        general_game_updates,
        team_a_updates,
        team_b_updates,
        m["description"]);
}

string getUser(string message)
{
    vector<string> lines;
    split(lines,message,is_any_of("\n"));

    for(string line : lines)
    {
        vector<string> words;
        split(words,line,is_any_of(":"));
        if(words[0] == "user") {return words[1];}
    }
    return "-1";
}

void StompProtocol::process()
{
    vector<string> lines;
    string input;

    while (mClient->getState() | mClient->getWait())
    {
        mConnectionHandler->getLine(input);
        split(lines,input,is_any_of("\n"));
        map<string,string> map = convertStringToMap(lines);

        if (map["stomp-command"] == "CONNECTED")
        {
            mClient->setState(true);
            mClient->setWait(false);
            cout << "Login successful" <<endl;  
            cout << "----------" <<endl;
        }

        if (map["stomp-command"] == "MESSAGE")
        {

            string user = getUser(map["message"]);
            if (user == "-1") {cout << "User header not found" <<endl;}
            else
            {
                pair<string,string> user_and_channel = {user,map["destination"]};
                Event event = convertStringToEvent(map["message"]);
                mReports->addReport(user_and_channel,event);
                cout << map["message"] <<endl;
                cout << "----------" <<endl;
            }
        }
        else if (map["stomp-command"] == "ERROR")
        {
            replace_all(input, "\n\n","");
            cout << input <<endl; 
            mClient->setState(false);
            mConnectionHandler->close();
            cout << "The user disconnected from the server" <<endl; 
            cout << "----------" <<endl;
            break;
        }
        else if (map["stomp-command"] == "RECEIPT")
        {
            string command = mClient->getReceipt(map["receipt-id"]);
            if (command == "empty-string") {cout << "This receipt is not in the database" <<endl;}
            else
            {
                vector<string> parts;
                split(parts,command,is_any_of(" "));
                
                if (parts[0] == "join")
                {
                    mClient->addIdAndChannel(parts[1],mClient->getIdCounter());
                    mClient->raiseByOneIdCounter();
                    cout << "Joined channel " + parts[1] <<endl;
                    cout << "----------" <<endl;
                }
                else if (parts[0] == "exit")
                {
                    mClient->removeIdByChannel(parts[1]);
                    cout << "Exited channel " + parts[1] <<endl;
                    cout << "----------" <<endl;
                }
                else if (parts[0] == "logout")
                {
                    mClient->setState(false);
                    mConnectionHandler->close();
                    cout << "The user disconnected from the server" <<endl; 
                    cout << "----------" <<endl;
                    break;
                }
            }
        }
        lines.clear();
        input = "";
    }
}


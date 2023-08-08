#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "ConnectionHandler.h"
#include "StompProtocol.h"
#include "Frame.h"
#include "Client.h"
#include "event.h"
#include "Reports.h" 

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <thread>

using namespace std;
using boost::split;
using boost::is_any_of;
using boost::lexical_cast;

int main(int argc, char *argv[]) 
{
    bool login = false;
    const short bufsize = 1024;
    char buf[bufsize];
    vector<string> msg_from_keyboard;
    vector<string> host_port;
    Frame message = Frame(); 

    while(true)
    {
        if (!login)
        {
            cin.getline(buf, bufsize);
            string command(buf);
            split(msg_from_keyboard,command,is_any_of(" "));
        }

        if ((msg_from_keyboard.size() < 4) | (msg_from_keyboard[0] != "login"))
        {
            cerr << "A login command is required" << endl;
            cerr << "login {host:port} {username} {password}" << endl;
            cout << "----------" <<endl;
            return -1;
        }

        split(host_port,msg_from_keyboard[1],is_any_of(":"));
        string host = host_port[0];
        short port = boost::lexical_cast<short>(host_port[1]); 
        ConnectionHandler connectionHandler(host, port); //------------------------------//
        if (!connectionHandler.connect()) {cerr << "Cannot connect to " << host << ":" << port << endl; return -1;}

        Client client = Client(msg_from_keyboard[2]); //------------------------------//
        Reports reports = Reports(); //------------------------------//
        string msg_for_server = message.connect(msg_from_keyboard[2],msg_from_keyboard[3],"stomp.cs.bgu.ac.il","1.2");
        connectionHandler.sendLine(msg_for_server);

        StompProtocol stompProtocol(&connectionHandler,&client,&reports); //------------------------------//
        thread stompProtocolThread(&StompProtocol::process,&stompProtocol); //------------------------------//

        while ((client.getState() | client.getWait()))
        {         
            msg_from_keyboard.clear();
            msg_for_server = "";
            login = false;

            cin.getline(buf, bufsize);
            string command(buf);
            split(msg_from_keyboard,command,is_any_of(" "));

            if (msg_from_keyboard[0] == "login") {login = true; break;}

            if ((msg_from_keyboard[0] == "join") & (client.getState())) // join {game_name}
            {
                msg_for_server = message.subscribe(msg_from_keyboard[1],client.getIdCounter(),client.getReceiptCounter());
                client.addReceipt(to_string(client.getReceiptCounter()),command);
                client.raiseByOneReceiptCounter();
                connectionHandler.sendLine(msg_for_server);
            }
            else if ((msg_from_keyboard[0] == "exit") & (client.getState())) // exit {game_name} - throwing an error frame
            { 
                msg_for_server = message.unsubscribe(client.getIdByChannel(msg_from_keyboard[1]),client.getReceiptCounter());
                client.addReceipt(to_string(client.getReceiptCounter()),command);
                client.raiseByOneReceiptCounter();
                connectionHandler.sendLine(msg_for_server);
            }
            else if ((msg_from_keyboard[0] == "report") & (client.getState())) // report {file} - throwing an error frame
            {
                names_and_events events_and_names = parseEventsFile(msg_from_keyboard[1]);
        
                for (Event event : events_and_names.events)
                {
                msg_for_server = message.send(client.getUsername(),
                    events_and_names.team_a_name,
                    events_and_names.team_b_name,
                    event); 

                    connectionHandler.sendLine(msg_for_server); 
                }
            }
            else if ((msg_from_keyboard[0] == "summary") & (client.getState())) // summary {game_name} {user} {file} - throwing an error message
            {
                vector<string> team_a_and_team_b;
                split(team_a_and_team_b,msg_from_keyboard[1],is_any_of("_"));

                string team_a = team_a_and_team_b[0];
                string team_b = team_a_and_team_b[1];

                pair<string,string> user_and_channel = {client.getUsername(),msg_from_keyboard[1]};
                vector<Event> events;
                if (!reports.getReports(user_and_channel, &events)) 
                {
                    cout << "User or game is not found in the database" <<endl;
                    cout << "----------" <<endl;
                }
                else
                {
                    msg_for_server = message.summary(team_a,team_b,events);

                    fstream fileStream;
                    fileStream.open(msg_from_keyboard[3], ofstream::out | ofstream::trunc);
                    if (fileStream.fail()) {ofstream fileStream(msg_from_keyboard[3]);}
                    fileStream << msg_for_server;
                    fileStream.close();
        
                    cout << msg_for_server <<endl;
                }
            }   
            else if ((msg_from_keyboard[0] == "logout") & (client.getState())) // logout
            {
                msg_for_server = message.disconnect(client.getReceiptCounter());
                client.addReceipt(to_string(client.getReceiptCounter()),command);
                client.raiseByOneReceiptCounter();
                connectionHandler.sendLine(msg_for_server);
                stompProtocolThread.join();
                break;
            }
            else
            {
                cout << "Unknown command" <<endl; // we assume the input is correct
                client.setState(false);
                connectionHandler.close();
                cout << "The user disconnected from the server" <<endl; 
                cout << "----------" <<endl;
                break;
            }
        }       
        if (msg_from_keyboard[0] != "logout") {stompProtocolThread.join();}
    }
    return 0;
}
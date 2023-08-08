#include "Frame.h"
#include <boost/algorithm/string.hpp>
#include <string>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::to_string;
using boost::split;
using boost::is_any_of;

Frame::Frame() {}

Frame::~Frame() {}

string Frame::connect(string username, string password, string host, string acceptVersion)
{
        return 
        "CONNECT\naccept-version:" + acceptVersion + "\n" +
        "host:" + host + "\n" +
        "login:" + username + "\n" +
        "passcode:" + password + "\n" +
        "\n" + "\0";
}

string Frame::disconnect(int receipt)
{
        return
        "DISCONNECT\nreceipt:" + to_string(receipt) + "\n" +
        "\n" + "\0";
}

string Frame::send(string username, string team_a_name, string team_b_name, Event &event)
{
        string event_name = event.get_name();
        int time = event.get_time();
        map<string,string> game_updates = event.get_game_updates();
        map<string,string> team_a_updates = event.get_team_a_updates();
        map<string,string> team_b_updates = event.get_team_b_updates();
        string description = event.get_discription();

        string game_update = "";
        for (pair<string,string> p : game_updates) 
        {
                game_update = game_update + "     " + p.first + ":" + p.second + "\n";
        }

        string team_a_update = "";
        for (pair<string,string> p : team_a_updates) 
        {
                team_a_update = team_a_update + "     " + p.first + ":" + p.second + "\n";
        }

        string team_b_update = "";
        for (pair<string,string> p : team_b_updates) 
        {
                team_b_update = team_b_update + "     " + p.first + ":" + p.second + "\n";
        }
  
        return
        "SEND\ndestination:" + team_a_name + "_" + team_b_name + "\n\n" +
        "user:" + username + "\n" +
        "team a:" + team_a_name + "\n" +
        "team b:" + team_b_name + "\n" +
        "event name:" + event.get_name() + "\n" +
        "time:" + to_string(time) + "\n" +
        "general game updates:" + "\n" + game_update +
        "team a updates:" + "\n" + team_a_update +
        "team b updates:" + "\n" + team_b_update +
        "description:" + "\n" + event.get_discription() + "\0";
}

string Frame::subscribe(string destination, int id, int receipt)
{
        return
        "SUBSCRIBE\ndestination:" + destination + "\n" +
        "id:" + to_string(id) + "\n" +
        "receipt:" + to_string(receipt) + "\n" +
        "\n" + "\0";
}

string Frame::unsubscribe(int id,int receipt)
{
        return
        "UNSUBSCRIBE\nid:" + to_string(id) + "\n" +
        "receipt:" + to_string(receipt) + "\n" +
        "\n" + "\0";
}

string Frame::summary(string team_a_name, string team_b_name, vector<Event> &events)
{
        string general_stats = "";
        string team_a_stats = "";
        string team_b_stats = "";
        string time_name_description = "";

        Event event = events.back(); //

        for (pair<string,string> p : event.get_game_updates()) {general_stats = general_stats + p.first + ":" + p.second + "\n";}
        for (pair<string,string> p : event.get_team_a_updates()) {team_a_stats = team_a_stats + p.first + ":" + p.second + "\n";}
        for (pair<string,string> p : event.get_team_b_updates()) {team_b_stats = team_b_stats + p.first + ":" + p.second + "\n";}

        for (Event event : events)
        {
                time_name_description = time_name_description + 
                to_string(event.get_time()) + " - " + event.get_name() + "\n\n" +
                event.get_discription() + "\n\n";
        }

        return
        team_a_name + " vs " + team_b_name + "\n" +
        "Game state:" + "\n" +
        "General stats:" + "\n" + general_stats + "\n" +
        team_a_name + " stats:" + "\n" + team_a_stats + "\n" +
        team_b_name + " stats:" + "\n" + team_b_stats + "\n" +
        "Game event reports:" + "\n" + time_name_description + "\n";
}

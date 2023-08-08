#pragma once

#include <string>
#include <vector>

class Simulation;
class JoinPolicy;

using std::string;
using std::vector;

enum State
{
    Waiting,
    CollectingOffers,
    Joined
};

class Party
{
public:
    Party(int id, string name, int mandates, JoinPolicy *); 

    State getState() const;
    void setState(State state);
    int getMandates() const;
    void step(Simulation &s);
    const string &getName() const;

    const vector<int> getAgents() const;
    void addAgent(int agentId);
    const int getId() const;   
    const bool offerChecking(const int coalitionsId) const;
    void offerMarking(const int coalitionsId);

    virtual ~ Party(); //destructor
    Party(const Party &other); //copy constructor
    Party &operator=(const Party &other); //copy assignment operator
    Party(Party &&other); //move constructor
    Party &operator=(Party && other); //move assignment operator

private:
    int mId;
    string mName;
    int mMandates;
    JoinPolicy *mJoinPolicy;
    State mState;

    int mTimer; // init in party class
    vector<int> mAgents; // init in party class
    vector<bool> mCoalitions; // init in party class
    bool mLoadingCoalitionsVector; // init in party class - step function
};

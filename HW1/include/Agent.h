#pragma once

#include <vector>

using std::vector;

class Simulation;
class SelectionPolicy;

class Agent
{
public:
    Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy);

    int getPartyId() const;
    int getId() const;
    void step(Simulation &);

    const vector<int> &getParties() const;
    void addParty(int partyId);
    void removeParty(int partyId);
    const int getCoalitionId() const;
    void setCoalitionId(int coalitionId);

    virtual ~Agent(); //destructor
    Agent(const Agent &other); //copy constructor
    Agent &operator=(const Agent &other); //copy assignment operator
    Agent(Agent &&other); //move constructor
    Agent &operator=(Agent &&other); //move assignment operator

    Agent(const Agent &other,int agentId,int partyId); // another copy constructor

private:
    int mAgentId;
    int mPartyId;
    SelectionPolicy *mSelectionPolicy;

    vector<int> mParties; // init in simulation class
    int mCoalitionId; // init in simulation class
};

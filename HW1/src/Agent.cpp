#include "Agent.h"
#include "Simulation.h"
#include "SelectionPolicy.h"

Agent::Agent(int agentId, int partyId, SelectionPolicy *selectionPolicy) : mAgentId(agentId), mPartyId(partyId),
 mSelectionPolicy(selectionPolicy), mParties(0), mCoalitionId(-1)
{

}

int Agent::getId() const
{
    return mAgentId;
}

int Agent::getPartyId() const
{
    return mPartyId;
}

const vector<int> &Agent::getParties() const
{
    return mParties;
}

void Agent::addParty(int partyId)
{
    mParties.push_back(partyId);
}

void Agent::removeParty(int partyId)
{
    int i = 0;

    for (int pId : mParties)
    {
        if (pId == partyId)
        {
            mParties.erase(mParties.begin() + i);
            break;
        }
        else {i++;}              
    }
}

const int Agent::getCoalitionId() const
{
    return mCoalitionId;
}

void Agent::setCoalitionId(int coalitionId)
{
    mCoalitionId = coalitionId;
}

Agent::Agent(const Agent &other,int agentId,int partyId) : mAgentId(other.mAgentId), mPartyId(other.mPartyId),
 mSelectionPolicy(other.mSelectionPolicy), mParties(), mCoalitionId(other.mCoalitionId)
{
    mAgentId = agentId;
    mPartyId = partyId;
    mSelectionPolicy = other.mSelectionPolicy->clone();
    mParties.clear();
    mCoalitionId = other.mCoalitionId;
}

void Agent::step(Simulation &sim)
{   
    if (!mParties.empty()) {(*mSelectionPolicy).select(sim.getGraph(),*this,mParties);}
}

///------------Rule of 5-------------///

//--------Destructor--------//
Agent::~Agent()
{
    if (mSelectionPolicy)
    {
        delete mSelectionPolicy;
        mSelectionPolicy = nullptr;
    }

    mParties.clear();
}

//---------------- copy constructor-------------//
// It is called when a new object is created from an existing object, as a copy of the existing object.
Agent::Agent(const Agent &other) : mAgentId(other.mAgentId), mPartyId(other.mPartyId),
 mSelectionPolicy(other.mSelectionPolicy->clone()), mParties(), mCoalitionId(other.mCoalitionId)
{
    for (int party: other.mParties)
    {
        mParties.push_back(party);
    }
}

//-------------copy assignment operator------------//
// This operator is called when an already initialized object is assigned a new value from another existing object. 
Agent &Agent::operator=(const Agent &other)
{
    if(this == &other) {return *this;}
    else
    {
        mAgentId = other.mAgentId;
        mPartyId = other.mPartyId;

        delete mSelectionPolicy;
        mSelectionPolicy = nullptr;
        mSelectionPolicy = other.mSelectionPolicy->clone();
        
        mParties.clear();
        for (int party: other.mParties)
        {
            mParties.push_back(party);
        }  

        mCoalitionId = other.mCoalitionId;
        return *this;
    }
}

//-----------------move constructor-----------//
// a move constructor is called on an object created by the operation.
Agent::Agent(Agent &&other): mAgentId(other.mAgentId), mPartyId(other.mPartyId),
 mSelectionPolicy(other.mSelectionPolicy->clone()), mParties(), mCoalitionId(other.mCoalitionId)
{
    for (int party: other.mParties)
    {
        mParties.push_back(party);
    }
}

//------------- move assignment operator ---------//
// a move assignment operator is called on an existing object.
Agent &Agent::operator=(Agent &&other)
{
    if(this == &other) {return *this;}
    else
    {
        mAgentId = other.mAgentId;
        mPartyId = other.mPartyId;

        delete mSelectionPolicy;
        other.mSelectionPolicy = nullptr;
        mSelectionPolicy = other.mSelectionPolicy->clone();

        mParties.clear();
        for (int party: other.mParties)
        {
            mParties.push_back(party);
        }    

        mCoalitionId = other.mCoalitionId;
        return *this;
    }
}

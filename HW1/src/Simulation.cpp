#include "Simulation.h"

#include <iostream>
using namespace std;

Simulation::Simulation(Graph graph, vector<Agent> agents) : mGraph(graph), mAgents(agents), mCoalitions(0), mIndex(0)
{
    for (Agent &agent : mAgents)
    {

        for (int j = 0; j < mGraph.getNumVertices(); j++)
        {
            if (mGraph.getEdgeWeight(agent.getPartyId(),j) > 0)
            {
                if (mGraph.getParty(j).getState() != Joined)
                {
                    agent.addParty(j); // partyId = partyIndex
                }
            }
        }

        agent.setCoalitionId(mIndex); mIndex++;
        mCoalitions.push_back(mGraph.getMandates(agent.getPartyId()));
    }   
}

const int Simulation::getIndex() const
{
    return mIndex;
}

void Simulation::step()
{
    for (int i = 0; i < mGraph.getNumVertices(); i++)
    {
        mGraph.getParty(i).step(*this);
    }

    for (Agent &agent : mAgents)
    {
        agent.step(*this);
    }
}

bool Simulation::shouldTerminate() const
{   
    for (const int coalition : mCoalitions)
    {
        if (coalition >= 61) {return true;}
    }

    for (int i = 0; i < mGraph.getNumVertices(); i++)
    {
        if (mGraph.getParty(i).getState() != Joined) {return false;}
    }
    
    return true;
}

const Graph &Simulation::getGraph() const
{
    return mGraph;
}

const vector<Agent> &Simulation::getAgents() const
{
    return mAgents;
}

const Party &Simulation::getParty(int partyId) const
{
    return mGraph.getParty(partyId);
}

/// This method returns a "coalition" vector, where each element is a vector of party IDs in the coalition.
/// At the simulation initialization - the result will be [[agent0.partyId], [agent1.partyId], ...]
const vector<vector<int>> Simulation::getPartiesByCoalitions() const
{
    vector<vector<int>> coalitions;

    for (int i = 0; i < mIndex; i++)
    {
        vector<int> coalition;

        for (const Agent &agent : mAgents)
        {
            if (agent.getCoalitionId() == i)
            {
                coalition.push_back(agent.getPartyId());
            }
        }
        
        coalitions.push_back(coalition);
    }

    return coalitions;
}

Graph &Simulation::getGraph()
{
    return mGraph;
}

vector<int> &Simulation::getCoalitions()
{
    return mCoalitions;
}

void Simulation::addAgent(Agent &agent)
{
    mAgents.push_back(agent);
}

vector<Agent> &Simulation::getAgents()
{
    return mAgents;
}

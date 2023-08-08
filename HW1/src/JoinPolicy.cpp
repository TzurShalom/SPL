#include "../include/JoinPolicy.h"
#include "Agent.h"
#include "Graph.h"

JoinPolicy* MandatesJoinPolicy::clone() const
{
    return new MandatesJoinPolicy();
}

JoinPolicy* LastOfferJoinPolicy::clone() const
{
    return new LastOfferJoinPolicy();
}

// If the number of mandates is not unique, join the coalition that offered first.
void MandatesJoinPolicy::join(Party &party, Simulation &simulation)
{
    // select agent
    int m = 0;
    Agent *mSelectedAgent;

    for (int agent : party.getAgents())
    {
        Agent &a = simulation.getAgents()[agent];
        if (simulation.getCoalitions()[a.getCoalitionId()] > m)
        {
            m = simulation.getCoalitions()[a.getCoalitionId()];
            mSelectedAgent = &a;
        }
    }

    // update coalition
    simulation.getCoalitions()[(*mSelectedAgent).getCoalitionId()] += party.getMandates();

    // update party
    party.setState(Joined);

    // clone agent
    Agent clonedAgent = Agent(*mSelectedAgent,simulation.getAgents().size(),party.getId());
    Graph &graph = simulation.getGraph();
    for (int i = 0; i < graph.getNumVertices(); i++)
    {
        if (graph.getEdgeWeight(party.getId(),i) > 0)
        {
            if ((graph.getParty(i).getState() != Joined) & (!graph.getParty(i).offerChecking(clonedAgent.getCoalitionId())))
            {
                clonedAgent.addParty(i);
            }
        }
    }  
    simulation.addAgent(clonedAgent);
}

// Selects the coalition that made the last offer.
void LastOfferJoinPolicy::join(Party &party, Simulation &simulation)
{
    // select agent
    Agent *mSelectedAgent = &simulation.getAgents()[party.getAgents().back()];

    // update coalition
    simulation.getCoalitions()[(*mSelectedAgent).getCoalitionId()] += party.getMandates();

    // update party
    party.setState(Joined);

    // clone agent
    Agent clonedAgent = Agent(*mSelectedAgent,simulation.getAgents().size(),party.getId());
    Graph &graph = simulation.getGraph();
    for (int i = 0; i < graph.getNumVertices(); i++)
    {
        if (graph.getEdgeWeight(party.getId(),i) > 0)
        {
            if ((graph.getParty(i).getState() != Joined) & (!graph.getParty(i).offerChecking(clonedAgent.getCoalitionId())))
            {
                clonedAgent.addParty(i);
            }
        }
    }  
    simulation.addAgent(clonedAgent);
}
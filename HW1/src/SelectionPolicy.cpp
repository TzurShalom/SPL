#include "../include/SelectionPolicy.h"

SelectionPolicy* MandatesSelectionPolicy::clone() const
{
    return new MandatesSelectionPolicy();
}

SelectionPolicy* EdgeWeightSelectionPolicy::clone() const
{
    return new EdgeWeightSelectionPolicy();
}

// If the maximum mandates/edge weight is not unique, take the first party (with the lower id).
void MandatesSelectionPolicy::select(Graph &graph, Agent &agent, vector<int> &parties)
{
    // select party
    int m = 0;
    Party *mSelectedParty;

    for (int party : parties)
    {
        Party &p = graph.getParty(party);
        if (!p.offerChecking(agent.getCoalitionId()))
        {
            if (p.getMandates() > m)
            {
                m = p.getMandates();
                mSelectedParty = &p;
            }
        }
        else {agent.removeParty(party);}
    }

    if (!agent.getParties().empty()) // if no party is selected
    {
        // update party
         if ((*mSelectedParty).getState() == Waiting) {(*mSelectedParty).setState(CollectingOffers);}
        (*mSelectedParty).addAgent(agent.getId());

        // update agent
        agent.removeParty((*mSelectedParty).getId());

        // update coalition
        mSelectedParty->offerMarking(agent.getCoalitionId());
    }   
}

// If the maximum mandates/edge weight is not unique, take the first party (with the lower id).
void EdgeWeightSelectionPolicy::select(Graph &graph, Agent &agent, vector<int> &parties)
{
    // select party
    int w = 0;
    Party *mSelectedParty;

    for (int party : parties)
    {
        Party &p = graph.getParty(party);
        if (!p.offerChecking(agent.getCoalitionId()))
        {
            if (graph.getEdgeWeight(agent.getPartyId(),party) > w)
            {
                w = graph.getEdgeWeight(agent.getPartyId(),party);
                mSelectedParty = &p;
            }
        }
        else {agent.removeParty(party);}
    }
    
    if (!agent.getParties().empty()) // if no party is selected
    {
        // update party
         if ((*mSelectedParty).getState() == Waiting) {(*mSelectedParty).setState(CollectingOffers);}
        (*mSelectedParty).addAgent(agent.getId());

        // update agent
        agent.removeParty((*mSelectedParty).getId());

        // update coalition
        mSelectedParty->offerMarking(agent.getCoalitionId());
    }  
}
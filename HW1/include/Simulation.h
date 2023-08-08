#pragma once

#include <vector>
#include "Graph.h"
#include "Agent.h"

using std::string;
using std::vector;

class Simulation
{
public:
    Simulation(Graph g, vector<Agent> agents);

    void step();
    bool shouldTerminate() const;

    const Graph &getGraph() const;
    const vector<Agent> &getAgents() const;
    const Party &getParty(int partyId) const;
    const vector<vector<int>> getPartiesByCoalitions() const;

    Graph &getGraph();
    vector<int> &getCoalitions();
    void addAgent(Agent &agent);
    vector<Agent> &getAgents();
    const int getIndex() const;

private:
    Graph mGraph;
    vector<Agent> mAgents;

    vector<int> mCoalitions; // init in simulation class
    int mIndex; // init in simulation class
};

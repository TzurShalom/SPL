#pragma once

#include "Party.h"
#include "Agent.h"
#include "Graph.h"
#include <vector>

using std::vector;

class SelectionPolicy
{
public:
    virtual void select(Graph &graph, Agent &agent, vector<int> &parties) = 0;
    virtual SelectionPolicy* clone() const = 0;
    virtual ~ SelectionPolicy()  = default;
};

class MandatesSelectionPolicy: public SelectionPolicy
{ 
public:
    void select(Graph &graph, Agent &agent, vector<int> &parties) override;
    virtual SelectionPolicy* clone() const override;
    virtual ~MandatesSelectionPolicy() override = default;
};

class EdgeWeightSelectionPolicy: public SelectionPolicy
{
public:
    void select(Graph &graph, Agent &agent, vector<int> &parties);
    virtual SelectionPolicy* clone() const override;
    virtual ~EdgeWeightSelectionPolicy() override = default;
};
#pragma once

#include "Party.h"
#include "Simulation.h"
#include <vector>

using std::vector;
using std::pair;

class JoinPolicy 
{
public:
    virtual void join(Party &party, Simulation &simulation) = 0; 
    virtual JoinPolicy* clone() const = 0;
    virtual ~ JoinPolicy() = default;
};

class MandatesJoinPolicy : public JoinPolicy
{
public:
    void join(Party &party, Simulation &simulation) override;
    virtual JoinPolicy* clone() const override;
    virtual ~ MandatesJoinPolicy() override = default;
};

class LastOfferJoinPolicy : public JoinPolicy 
{
public:
    void join(Party &party, Simulation &simulation) override;
    virtual JoinPolicy* clone() const override;
    virtual ~ LastOfferJoinPolicy() override = default;;
};
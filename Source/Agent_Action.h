#ifndef AGENT_ACTION_H
#define AGENT_ACTION_H

#include "Zone.h"

class Agent_Action
{
    public:
        Agent_Action();
        virtual void step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities) =0;
        virtual double getResult();
        virtual std::string getName();
    protected:
        double getFutureDurationOfPresenceState(const std::vector<double> &activities) const;
        double result;
        std::string name;
    private:

};

#endif // AGENT_ACTION_H

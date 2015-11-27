#ifndef AGENT_ACTION_LIGHTS_H
#define AGENT_ACTION_LIGHTS_H

#include "Agent_Action.h"


class Agent_Action_Lights : public Agent_Action
{
    public:
        Agent_Action_Lights();

        void step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities);

    protected:

    private:

};

#endif // AGENT_ACTION_LIGHTS_H

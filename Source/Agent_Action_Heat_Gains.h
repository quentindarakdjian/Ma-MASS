#ifndef AGENT_ACTION_HEAT_GAINS_H
#define AGENT_ACTION_HEAT_GAINS_H

#include "Agent_Action.h"


class Agent_Action_Heat_Gains : public Agent_Action
{
    public:

        Agent_Action_Heat_Gains();
        void prestep(double clo, double metabolicRate);
        void step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities);
        void setup(int agentid);
        double getPMV();

    protected:
    private:
        int id;
        double clo;
        double metabolicRate;
        double ppd;
        double pmv;
        std::string idAsString;
};

#endif // AGENT_ACTION_HEAT_GAINS_H

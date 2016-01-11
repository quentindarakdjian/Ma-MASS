#ifndef AGENT_ACTION_HEATING_SETPOINT_H_INCLUDED
#define AGENT_ACTION_HEATING_SETPOINT_H_INCLUDED

#include "Agent_Action.h"

class Agent_Action_Heating_Setpoint : public Agent_Action
{
    public:
        Agent_Action_Heating_Setpoint();

        void setup(int age);
        void step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities);

    protected:

    private:
        double temperatureSetpointBase;
        int age;
        std::deque<double> outDoorTemperatures;

};

#endif // AGENT_ACTION_HEATING_SETPOINT_H_INCLUDED

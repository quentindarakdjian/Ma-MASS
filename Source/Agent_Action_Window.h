#ifndef AGENT_ACTION_WINDOW_H
#define AGENT_ACTION_WINDOW_H

#include "Agent_Action.h"
#include "Model_Windows.h"
#include "Zone.h"

class Agent_Action_Window : public Agent_Action {
    public:
        Agent_Action_Window();

        void setup(int windowID);
        void step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities);

    private:
        Model_Windows m_window;
        std::deque<double> outDoorTemperatures;

        double getPreviousDurationOfAbsenceState(const std::vector<double> &activities) const;
        double getCurrentDurationOfPresenceState(const std::vector<double> &activities) const;
};

#endif // AGENT_ACTION_WINDOW_H

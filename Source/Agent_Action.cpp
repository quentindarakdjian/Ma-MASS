// Copyright AI Environnement 2017

#include "SimulationConfig.h"
#include "Agent_Action.h"

Agent_Action::Agent_Action(){

}

double Agent_Action::getResult(){
    return result;
}

std::string Agent_Action::getName(){
    return name;
}

double Agent_Action::getFutureDurationOfPresenceState(const std::vector<double> &activities) const{
    double cdp = 0;
    int stepCount = SimulationConfig::getStepCount();
    unsigned int stepCounter = stepCount;
    int lengthOfTimeStepSeconds = (60 * (60 / SimulationConfig::info.timeStepsPerHour));

    while (stepCounter + 1 < activities.size() && activities.at(stepCount) == activities.at(stepCounter+1)){
        cdp = cdp + lengthOfTimeStepSeconds;
        stepCounter++;
    }
    return cdp;
}

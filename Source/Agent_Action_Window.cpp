#include <iostream>
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Agent_Action_Window.h"

Agent_Action_Window::Agent_Action_Window(){
    name = "Windows";
}

void Agent_Action_Window::setup(int windowID){
        windowStruct ws = SimulationConfig::windows.at(windowID);
        m_window.setDurationVars(ws.aop, ws.bopout, ws.shapeop);
        m_window.setArrivalVars(ws.a01arr,ws.b01inarr,ws.b01outarr,ws.b01absprevarr,ws.b01rnarr);
        m_window.setInterVars(ws.a01int,ws.b01inint,ws.b01outint,ws.b01presint,ws.b01rnint);
        m_window.setDepartureVars(ws.a01dep, ws.b01outdep, ws.b01absdep, ws.b01gddep, ws.a10dep, ws.b10indep, ws.b10outdep, ws.b10absdep, ws.b10gddep);
}

void Agent_Action_Window::step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities){

        double outdoorTemperature = DataStore::getValue("EnvironmentSiteOutdoorAirDrybulbTemperature");

        outDoorTemperatures.push_back(outdoorTemperature);
        if (outDoorTemperatures.size() > (SimulationConfig::info.timeStepsPerHour * 24)) {
                outDoorTemperatures.pop_front();
        }

        double rain = DataStore::getValue("EnvironmentSiteRainStatus");
        double indoorTemperature = zone.getMeanAirTemperature();
        double timeStepLengthInMinutes = SimulationConfig::lengthOfTimestep();

        m_window.setWindowState(zone.getWindowState());
        if (inZone && !previouslyInZone) {
                double previousDuration = getPreviousDurationOfAbsenceState(activities);
                m_window.arrival(indoorTemperature, outdoorTemperature, previousDuration, rain, timeStepLengthInMinutes);
        } else if ((inZone && previouslyInZone )) {
                double currentDuration = getCurrentDurationOfPresenceState(activities);
                m_window.intermediate(indoorTemperature, outdoorTemperature, currentDuration, rain, timeStepLengthInMinutes);
        } else if ((!inZone && previouslyInZone )) {
                double dailyMeanTemperature = 0;
                for (double temp : outDoorTemperatures) {
                        dailyMeanTemperature += temp;
                }
                dailyMeanTemperature = dailyMeanTemperature / (double)outDoorTemperatures.size();

                double groundFloor = zone.getGroundFloor();
                double futureDuration = getFutureDurationOfPresenceState(activities);
                m_window.departure(indoorTemperature, dailyMeanTemperature, futureDuration, groundFloor);
        }

        result = m_window.getWindowState();


}

double Agent_Action_Window::getPreviousDurationOfAbsenceState(const std::vector<double> &activities) const {
        double cdp = 0;
        int stepCount = SimulationConfig::getStepCount();
        int stepCounter = stepCount;
        int lengthOfTimeStepSeconds = (60 * (60 / SimulationConfig::info.timeStepsPerHour));
        while (stepCounter > 0 && activities.at(stepCount) != activities.at(stepCounter-1)) {
                cdp = cdp + lengthOfTimeStepSeconds;
                stepCounter--;
                if(stepCounter < 1){
                    break;
                }
        }
        return cdp;
}

double Agent_Action_Window::getCurrentDurationOfPresenceState(const std::vector<double> &activities) const {
        double cdp = 0;
        int stepCount = SimulationConfig::getStepCount();
        int stepCounter = stepCount;
        int lengthOfTimeStepSeconds = (60 * (60 / SimulationConfig::info.timeStepsPerHour));
        while(activities.at(stepCount) == activities.at(stepCounter-1)) {
                cdp = cdp + lengthOfTimeStepSeconds;
                stepCounter--;
                if(stepCounter < 1){
                    break;
                }
        }
        return cdp;
}


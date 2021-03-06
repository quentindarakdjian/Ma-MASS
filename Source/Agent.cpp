// Copyright AI Environnement 2017

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <limits>
#include <memory>

#include "Model_HeatGains.h"
#include "Model_Lights.h"
#include "Model_HeatingSetpoint.h"
#include "DataStore.h"
#include "SimulationConfig.h"
#include "StateMachine.h"
#include "Utility.h"

#include "Agent.h"

Agent::Agent(){
}

/**
 * @brief Agent creation
 * @details The function recovers agents properties defined in SimulationConfig
 */
Agent::Agent(int newId) : id(newId){
    std::string idAsString = std::to_string(newId);
    DataStore::addVariable("Agent_Activity_" + idAsString);
    agentStruct agent = SimulationConfig::agents.at(id);
    jobType = agent.jobType; //For office building simulations
    bedroom = agent.bedroom;
    office = agent.office;
    power = agent.power;
    age = agent.age;
    aahg.setup(id);
    availableActions.push_back(0);
    if (SimulationConfig::info.windows){
        aaw.setup(agent.windowId);
        availableActions.push_back(1);
    }
    if (SimulationConfig::info.shading){
        aas.setup(agent.shadeId);
        availableActions.push_back(2);
    }
    if (SimulationConfig::info.lights){
        availableActions.push_back(3);
    }
    if (SimulationConfig::info.heatingSetpoint){
        aahs.setup(age);
        availableActions.push_back(4);
    }
    if (SimulationConfig::info.presencePage){
      model_presenceFromPage();
    }
    else{
      model_presenceFromActivities();
    }
}

void Agent::step(StateMachine *stateMachine){

    int stepCount = SimulationConfig::getStepCount();
    int newStateID = activities.at(stepCount);
    previousState = state;
    state = stateMachine->transistionTo(newStateID);
    Zone* zonePtr = updateLocation(state);
    metabolicRate = state.getMetabolicRate();
    clo = state.getClo();
    activity = state.getActivity();

    if(presentAt(stepCount)){
        interactWithZone(*zonePtr);
    }
    if( stepCount > 0 && previousState.getId() != state.getId() && presentAt(stepCount - 1)){
        zonePtr = updateLocation(previousState);
        interactWithZone(*zonePtr);
    }
    std::string name = "Agent_Activity_" + std::to_string(id);
    DataStore::addValue(name.c_str(), activities.at(stepCount));
}

void Agent::actionStep(int action, ActionValues *interaction, const Zone &zone, bool inZone, bool preZone){
    switch (action) {
      case 0:
            aahg.prestep(clo, metabolicRate);
            aahg.step(zone, inZone, preZone, activities);
            interaction->heatgains = aahg.getResult();
            previous_pmv = pmv;
            pmv = aahg.getPMV();
        break;
      case 1:
            aaw.step(zone, inZone, preZone, activities);
            interaction->windowState = aaw.getResult();
        break;
      case 2:
            aas.step(zone, inZone, preZone, activities);
            interaction->shadeState = aas.getResult();
        break;
      case 3:
            aal.step(zone, inZone, preZone, activities);
            interaction->lightState = aal.getResult();
        break;
      case 4:
            aahs.step(zone, inZone, preZone, activities);
            interaction->heatingSetpointState = aahs.getResult();
        break;
      }
}

void Agent::interactWithZone(const Zone &zone){
    ActionValues interaction; //ActionValues is the struct
    bool inZone = currentlyInZone(zone);
    bool preZone = previouslyInZone(zone);

    std::random_shuffle (availableActions.begin(), availableActions.end() );
    for(int a : availableActions) {
        actionStep(a, &interaction, zone, inZone, preZone);
    }
    zoneToInteraction[zone.getName()] = interaction;
}

void Agent::model_activity(){
    Model_Activity ma;
    activities = ma.preProcessActivities(id);
}

void Agent::model_presenceFromActivities(){
    model_activity();
    presence.calculatePresenceFromActivities(activities);
}

void Agent::model_presenceFromPage(){
    presence.calculatePresenceFromPage(id);
    for (unsigned int i = 0; i < presence.size(); ++i){
        if (presence.at(i)){
            activities.push_back(3);
        }
        else{
            activities.push_back(9);
        }
    }
}

void Agent::model_pastAndFutureDurations(){
    int lengthOfTimeStepSeconds = (60 * (60 / SimulationConfig::info.timeStepsPerHour));
    currentDurationOfPresenceState.push_back(0);

    for (unsigned int i = 1; i < presence.size(); ++i){
        // std::cout << occupantFractions.at(i) << std::endl;
        bool currentPresence = presence.at(i) > 0;
        bool previousPresence = presence.at(i - 1) > 0;
        if (currentPresence == previousPresence){
            currentDurationOfPresenceState.push_back(currentDurationOfPresenceState[i - 1] + lengthOfTimeStepSeconds);
        }
        else{
            currentDurationOfPresenceState.push_back(0);
        }
    }
    presenceForFutureSteps.assign(presence.size(), std::numeric_limits<int>::quiet_NaN());
    for (unsigned int i = 1; i+1 < presence.size(); ++i){
        bool currentPresence = presence.at(i-1) > 0;
        bool futurePresence = presence.at(i) > 0;
        if (currentPresence && !futurePresence){
            unsigned int j = i + 1;
            while (presence.at(j) == 0 && j < presence.size()){
                j++;
                if (j >= presence.size()){
                    break;
                }
            }
            presenceForFutureSteps[i] = currentDurationOfPresenceState[j - 1];
        }
    }
}

bool Agent::presentAt(int step) const{
    return presence.at(step);
}

double Agent::getCurrentRadientGains(const Zone &zone) const{
    return zoneToInteraction.at(zone.getName()).heatgains;
}

double Agent::getPower() const{
    return power;
}

int Agent::getAge() const{
    return age;
}

bool Agent::getDesiredLightState(const Zone &zone) const{
    return zoneToInteraction.at(zone.getName()).lightState;
}

bool Agent::getDesiredWindowState(const Zone &zone) const{
    return zoneToInteraction.at(zone.getName()).windowState;
}

bool Agent::getDesiredShadeState(const Zone &zone) const{
    return zoneToInteraction.at(zone.getName()).shadeState;
}

double Agent::getDesiredHeatingSetpointState(const Zone &zone) const{
    return zoneToInteraction.at(zone.getName()).heatingSetpointState;
}

bool Agent::currentlyInZone(const Zone &zone) const{
    return zone.getName() == updateLocation(state)->getName();
}

bool Agent::previouslyInZone(const Zone &zone) const{
    bool inZone = false;
    if (SimulationConfig::getStepCount() > 0){
        inZone = zone.getName() == updateLocation(previousState)->getName();
    }
    return inZone;
}

bool Agent::InteractionOnZone(const Zone &zone) const{
    return currentlyInZone(zone) || previouslyInZone(zone);
}

int Agent::presentForFutureSteps() const{
    return presenceForFutureSteps.at(SimulationConfig::getStepCount());
}

bool Agent::isInZone(std::string zoneName) const{
    return (updateLocation(state)->getName() == zoneName);
}

std::string Agent::getLocationType(int step, StateMachine *sm){
    int newStateID = activities.at(step);
    State s = sm->transistionTo(newStateID);
    // std::cout << "Location is: " << s.getType() <<std::endl;
    return s.getActivity();
}




Zone* Agent::updateLocation(const State& s) const{

    std::vector<Zone*> zonePtrs = s.getZonesPtr();
    Zone* zone = zonePtrs.at(0);
    if (s.getActivity() == "Sleeping" || s.getActivity() == "Sleep" ){
      for(Zone* &z : zonePtrs){
        if(z->getName() == bedroom){
          zone = z;
        }
      }
    }
    else if(s.getActivity() == "IT" && SimulationConfig::info.presencePage){
        if(office != ""){
          for(Zone* &z : zonePtrs){
            if(z->getName() == office){
              zone = z;
            }
          }
        }
    }
    return zone;
}

void Agent::postprocess(){
}

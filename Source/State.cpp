
#include "SimulationConfig.h"
#include "State.h"
#include "StateMachine.h"

StateMachine stateMachine;

State::State(){}
State::~State(){}

int State::getId() const{
    return id;
}

double State::getMetabolicRate()const{
    return metabolicRate;
}

double State::getClo()const{
    return clo;
}

std::vector<std::string> State::getLocation() const{
    return location;
}

std::string State::getActivity() const{
    return activity;
}

std::vector<std::string> State::getLocationFromActivty(const std::string *activty){
    return SimulationConfig::getZoneNameFromActivity(*activty);
}

void State::addState(State s){
	stateMachine.addState(s);
}

bool State::hasState(const int stateID){
	return stateMachine.hasState(stateID);
}

State State::getNextState(){
    return stateMachine.getPendingState();
}

std::vector<Zone*> State::getZonesPtr() const{
    return zones;
}

void State::addZonePtr(Zone* zoneptr){
    zones.push_back(zoneptr);
}

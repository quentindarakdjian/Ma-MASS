
#include <fstream>
#include <iostream>
#include "DataStore.h"
#include "SimulationConfig.h"
#include "Zone.h"

Zone::Zone() {

}

Zone::Zone(std::string zoneName) : name(zoneName) {
    //stepCount = -1;
    occupantFraction = 0;
    currentAgentGains = 0;
    blindState = 1;
    lightState = 0;
    heatingSetpointState = 20;
    windowState = 0;
    setActive(SimulationConfig::activeZone(&name));
    if(active){
        setGroundFloor(SimulationConfig::isZoneGroundFloor(&name));
        variableNameWindow = name + "WindowState";
        DataStore::addVariable(variableNameWindow);
        variableNameBlindFraction = name + "BlindFraction";
        DataStore::addVariable(variableNameBlindFraction);
        variableNameLight = name + "LightState";
        DataStore::addVariable(variableNameLight);
        variableNameHeatingSetpoint = name + "HeatingSetpointState";
        DataStore::addVariable(variableNameHeatingSetpoint);
        variableNameNumberOfOccupants = name + "NumberOfOccupants";
        DataStore::addVariable(variableNameNumberOfOccupants);
        variableNameAverageGains = name + "AverageGains";
        DataStore::addVariable(variableNameAverageGains);
    }
}

void Zone::setup(){
}

void Zone::step() {
    //stepCount++;
    if(active){
        DataStore::addValue(variableNameNumberOfOccupants, occupantFraction);
        DataStore::addValue(variableNameAverageGains, currentAgentGains);
        DataStore::addValue(variableNameWindow, windowState);
        DataStore::addValue(variableNameBlindFraction, blindState);
        DataStore::addValue(variableNameLight, lightState);
        DataStore::addValue(variableNameHeatingSetpoint, heatingSetpointState);
    }
}

void Zone::setName(std::string name) {
    this->name = name;
}

std::string Zone::getName() const {
    return name;
}


double Zone::getMeanAirTemperature() const {
    return DataStore::getValue(name + "ZoneMeanAirTemperature");
}

double Zone::getAirRelativeHumidity() const {
    return DataStore::getValue(name + "ZoneAirRelativeHumidity");
}

double Zone::getMeanRadiantTemperature() const {
    return DataStore::getValue(name + "ZoneMeanRadiantTemperature");
}

double Zone::getDaylightingReferencePoint1Illuminance() const {
    return DataStore::getValue(name + "DaylightingReferencePoint1Illuminance");
}

double Zone::getAirSystemSensibleHeatingRate() const {
    return DataStore::getValue(name + "ZoneAirSystemSensibleHeatingRate");
}


void Zone::setCurrentAgentGains(double currentAgentGains) {
    this->currentAgentGains = currentAgentGains;
}

double Zone::getCurrentAgentGains() const {
    return currentAgentGains;
}

void Zone::setOccupantFraction(double occupantFraction) {
    this->occupantFraction = occupantFraction;
}

float Zone::getOccupantFraction() const {
    return occupantFraction;
}

void Zone::setActive(bool active) {
    this->active = active;
}

bool Zone::isActive() const {
    return active;
}

void Zone::setGroundFloor(bool groundFloor) {
    this->groundFloor = groundFloor;
}

double Zone::getGroundFloor() const {
    return groundFloor;
}


void Zone::setWindowState(bool windowState){
    this->windowState = windowState;
}

void Zone::setLightState(bool lightState) {
    this->lightState = lightState;
}

void Zone::setBlindState(double blindState){
    this->blindState = blindState;
}

void Zone::setHeatingSetpointState(double heatingSetpointState) {
    this->heatingSetpointState = heatingSetpointState;
}

double Zone::getWindowState() const {
    return windowState;
}

double Zone::getLightState() const {
    return lightState;
}

double Zone::getBlindState() const {
    return blindState;
}

double Zone::getHeatingSetpointState() const {
    return heatingSetpointState;
}




#include <iostream>
#include "SimulationConfig.h"
#include "EP_ReadIDF.h"
#include "Model_EnergyPlus.h"

Model_EnergyPlus::Model_EnergyPlus() {
}

void Model_EnergyPlus::setup() {

    epIDFReader.loadIDF();

    SimulationConfig::info.timeSteps = epIDFReader.timeSteps();
    SimulationConfig::info.timeStepsPerHour = epIDFReader.timeStepsPerHour();
    SimulationConfig::info.startDay = epIDFReader.getBeginDay();
    SimulationConfig::info.startMonth = epIDFReader.getBeginMonth();
    SimulationConfig::info.endDay = epIDFReader.getEndDay();
    SimulationConfig::info.endMonth  = epIDFReader.getEndMonth();
}

void Model_EnergyPlus::beforeStep() {
}

void Model_EnergyPlus::step() {
}

std::vector<Zone> Model_EnergyPlus::getZones() {
    std::vector<Zone> zones = epIDFReader.getZones();
    // Add the external zone
    bool addExternal = true;
    for(Zone z: zones){
        if (z.getName() == "out") {
            addExternal = false;
            break;
        }
    }
    if(addExternal){
        zones.push_back(Zone("Out"));
    }
    return zones;
}

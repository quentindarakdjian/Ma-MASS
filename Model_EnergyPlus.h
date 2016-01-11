/*
 * File:   Comp_EnergyPlus.h
 * Author: jake
 *
 * Created on September 13, 2013, 11:51 AM
 */

#ifndef COMP_ENERGYPLUS_H
#define COMP_ENERGYPLUS_H
#include <vector>
#include "Zone.h"
#include "EP_ReadIDF.h"

class Model_EnergyPlus {
public:
    Model_EnergyPlus();
    void setup();
    void beforeStep();
    void step();
    std::vector<Zone> getZones();

private:
    int time;
    std::vector<int> monthCount;
    EP_ReadIDF epIDFReader;

};

#endif  /* COMP_ENERGYPLUS_H */

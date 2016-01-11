/*
 * File:   EP_ReadIDF.h
 * Author: jake
 *
 * Created on September 18, 2013, 10:31 AM
 */

#ifndef EP_READIDF_H
#define	EP_READIDF_H
#include <vector>
#include "EP_IDFObject.h"
#include "Zone.h"


class EP_ReadIDF {
public:
    EP_ReadIDF();

    void loadIDF();
    int timeSteps();
    int timeStepsPerHour();
    std::vector<std::string> getZoneNames() const;
    std::vector<Zone> getZones() const;
    int getEndDay() const;
    int getBeginDay() const;
    int getEndMonth() const;
    int getBeginMonth() const;
private:
    int getOrCreateZone(std::string name);
    bool processLine(std::stringstream* ss, std::string line);
    std::vector<Zone> zones;
    std::vector<EP_IDFObject> idfObjs;
    int beginMonth;
    int endMonth;
    int beginDay;
    int endDay;
    int timeStepPerHour;
    int numberOfTimeSteps;

};

#endif	/* EP_READIDF_H */


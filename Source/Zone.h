/*
 * File:   Zone.h
 * Author: jake
 *
 * Created on September 24, 2013, 10:11 AM
 */

#ifndef ZONE_H
#define	ZONE_H
#include "Model_Windows.h"
#include <string>
#include <vector>
#include <deque>

class Zone {
public:
    Zone();
    Zone(std::string);

    void step();
    void setup();
    void setName(std::string name);
    std::string getName() const;

    void setCurrentAgentGains(double currentAgentGains);
    double getCurrentAgentGains() const;
    double getWindowState() const;
    int getCurrentOccupantCount() const;
    float getOccupantFraction() const;
    double getBlindState() const;
    double getLightState() const;
    double getHeatingSetpointState() const;
    double getWindowDurationOpen() const;

    void setActive(bool active);
    bool isActive() const;
    void setGroundFloor(bool groundFloor);
    double getGroundFloor() const;

    double getAirSystemSensibleHeatingRate() const;
    double getMeanAirTemperature() const;
    double getAirRelativeHumidity() const;
    double getMeanRadiantTemperature() const;
    double getDaylightingReferencePoint1Illuminance() const;

    void setWindowDurationOpen(double windowDurationOpen);
    void setWindowState(bool windowState);
    void setLightState(bool lightState);
    void setHeatingSetpointState(double heatingSetpointState);
    void setBlindState(double state);
    void setOccupantFraction(double occupantFraction);

private:
    std::string name;
    bool groundFloor;
    double currentAgentGains;
    double blindState;
    bool lightState;
    double heatingSetpointState;
    bool windowState;
    double occupantFraction;

    std::string variableNameWindow;
    std::string variableNameBlindFraction;
    std::string variableNameNumberOfOccupants;
    std::string variableNameAverageGains;
    std::string variableNameLight;
    std::string variableNameHeatingSetpoint;

    bool active;

};

#endif	/* ZONE_H */


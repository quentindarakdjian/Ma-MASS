/*
 * File:   Comp_WindowsOpening.h
 * Author: jake
 *
 * Created on September 18, 2013, 10:19 AM
 */

#ifndef COMP_WINDOWSOPENING_H
#define COMP_WINDOWSOPENING_H
#include "Model_RandomWeibull.h"

class Model_Windows : Model_RandomWeibull{
public:
    Model_Windows();

    void setWindowState(bool state);
    bool getWindowState() const;

    void setDurationOpen(double durationOpen);
    double getDurationOpen() const;

    void arrival(double indoorTemperature, double outdoorTemperature, double previousDuration, bool rain, double timeStepLengthInMinutes );
    void intermediate(double indoorTemperature, double outdoorTemperature, double currentDuration, bool rain, double timeStepLengthInMinutes);
    void departure(double indoorTemperature, double dailyMeanTemperature, double futureDuration, double groundFloor);

    void setDurationVars(double aop, double bopout, double shapeop);
    void setArrivalVars(double a01arr, double b01inarr, double b01outarr, double b01absprevarr, double b01rnarr);
    void setInterVars(double a01int, double b01inint, double b01outint, double b01presint, double b01rnint);
    void setDepartureVars(double a01dep, double b01outdep, double b01absdep, double b01gddep, double a10dep, double b10indep, double b10outdep, double b10absdep, double b10gddep);

private:
    bool state;
    double durationOpen;

    double aop;
    double bopout;
    double shapeop;

    double a01arr;
    double b01inarr;
    double b01outarr;
    double b01absprevarr;
    double b01rnarr;
    // P01int
    double a01int;
    double b01inint;
    double b01outint;
    double b01presint;
    double b01rnint;
    // P01dep
    double a01dep;
    double b01outdep;
    double b01absdep;
    double b01gddep;
    // P10dep
    double a10dep;
    double b10indep;
    double b10outdep;
    double b10absdep;
    double b10gddep;

    double calculateDurationOpen(double outdoorTemperature);
};

#endif  /* COMP_WINDOWSOPENING_H */

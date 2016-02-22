/*
 * File:   Comp_WindowsOpening.cpp
 * Author: jake
 *
 * Created on September 18, 2013, 10:19 AM
 */
#include <cmath>
#include <deque>
#include <iostream>
#include "SimulationConfig.h"
#include "Model_Windows.h"

Model_Windows::Model_Windows() {
    durationOpen = 0;
    state = 0;

    aop = 2.151;
    bopout = 0.172;
    shapeop = 0.418;

    a01arr = -13.88;
    b01inarr = 0.312;
    b01outarr = 0.0433;
    b01absprevarr = 1.862;
    b01rnarr = -0.45;
    // P01int
    a01int = -12.23;
    b01inint = 0.281;
    b01outint = 0.0271;
    b01presint = -0.000878;
    b01rnint = -0.336;
    // P01dep
    a01dep = -8.75;
    b01outdep = 0.1371;
    b01absdep = 0.84;
    b01gddep = 0.83;
    // P10dep
    a10dep = -8.54;
    b10indep = 0.213;
    b10outdep = -0.0911;
    b10absdep = 1.614;
    b10gddep = -0.923;
}

void Model_Windows::setDurationVars(double aop, double bopout, double shapeop){
    this->aop = aop;
    this->bopout = bopout;
    this->shapeop = shapeop;
}

void Model_Windows::setArrivalVars(double a01arr, double b01inarr, double b01outarr, double b01absprevarr, double b01rnarr) {
    this->a01arr = a01arr;
    this->a01arr = a01arr;
    this->b01inarr = b01inarr;
    this->b01outarr = b01outarr;
    this->b01absprevarr = b01absprevarr;
    this->b01rnarr = b01rnarr;
}

void Model_Windows::setInterVars(double a01int, double b01inint, double b01outint, double b01presint, double b01rnint){
  this->a01int = a01int;
  this->b01inint = b01inint;
  this->b01outint = b01outint;
  this->b01presint = b01presint;
  this->b01rnint = b01rnint;
}

void Model_Windows::setDepartureVars(double a01dep, double b01outdep, double b01absdep, double b01gddep, double a10dep, double b10indep, double b10outdep, double b10absdep, double b10gddep){
  this->a01dep = a01dep;
  this->b01outdep = b01outdep;
  this->b01absdep = b01absdep;
  this->b01gddep = b01gddep;
  this->a10dep = a10dep;
  this->b10indep = b10indep;
  this->b10outdep = b10outdep;
  this->b10absdep = b10absdep;
  this->b10gddep = b10gddep;
}

void Model_Windows::setWindowState(bool state) {
    this->state = state;
}

bool Model_Windows::getWindowState() const{
    return state;
}

void Model_Windows::setDurationOpen(double durationOpen){
    this->durationOpen = durationOpen;
}

double Model_Windows::getDurationOpen() const{
    return durationOpen;
}

double Model_Windows::calculateDurationOpen(double outdoorTemperature){

    return randomWeibull(exp(aop + bopout * outdoorTemperature), shapeop);
}

void Model_Windows::arrival(double indoorTemperature, double outdoorTemperature, double previousDuration, bool rain, double timeStepLengthInMinutes) {

    double Rain = (rain ? 1.f : 0.f);

    //log(1/a)= 0.871
    if (!state) {
        float m = a01arr + b01inarr * indoorTemperature + b01outarr * outdoorTemperature + b01rnarr * Rain + b01absprevarr * ((previousDuration > 8.f * 60.f * 60.f) ? 1.f : 0.f);
        float prob01arr = exp(m) / (1.f + exp(m));
        double drand = randomDouble();
        if (drand < prob01arr) {
            durationOpen = calculateDurationOpen(outdoorTemperature);
            state = 1;
        } else {
            state = 0;
            durationOpen = 0.f;
        }
    } else {
        double durationOpen = calculateDurationOpen(outdoorTemperature);
        if (durationOpen < timeStepLengthInMinutes) {
            state = 0;
            durationOpen = 0;
        } else {
            state = 1;
            durationOpen = durationOpen - timeStepLengthInMinutes;
        }
    }

}

void Model_Windows::intermediate(double indoorTemperature, double outdoorTemperature, double currentDuration, bool rain, double timeStepLengthInMinutes) {

    double Rain = (rain ? 1.f : 0.f);


    if (!state) {
        double m = a01int + b01inint * indoorTemperature + b01outint * outdoorTemperature + b01presint * currentDuration + b01rnint * Rain;
        float prob01int = exp(m) / (1.f + exp(m));
        double drand = randomDouble();
        if (drand < prob01int) {
            durationOpen = calculateDurationOpen(outdoorTemperature);
            state = 1;
        } else {
            state = 0;
             durationOpen = 0.f;
        }
    } else {
        if (durationOpen < timeStepLengthInMinutes) {
            state = 0;
            durationOpen = 0.f;
        } else {
            state = 1;
            durationOpen = durationOpen - timeStepLengthInMinutes;
        }
    }

}

void Model_Windows::departure(double indoorTemperature, double dailyMeanTemperature, double futureDuration, double groundFloor ) {

    // double durationOpen;
    double drand = randomDouble();


    double durationLongerThanEightHours = 0.0;

    if (futureDuration >= 8.f * 60.f * 60.f) {
        durationLongerThanEightHours = 1.0;
    }
    if (!state) {
        double m = a01dep + b01outdep * dailyMeanTemperature + b01absdep * durationLongerThanEightHours + b01gddep * groundFloor;
        float prob01deplong = exp(m) / (1.f + exp(m));
        if (drand < prob01deplong) {
            state = 1;
        } else {
            state = 0;
            durationOpen = 0.f;
        }
    } else {
        double m = a10dep + b10indep * indoorTemperature + b10outdep * dailyMeanTemperature + b10absdep * durationLongerThanEightHours + b10gddep * groundFloor;
        float prob10deplong = exp(m) / (1.f + exp(m));
        if (drand < prob10deplong) {
            state = 0;
            durationOpen = 0.f;
        } else {
            state = 1;
        }
    }

}

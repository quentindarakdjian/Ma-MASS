/*
 * File:   Model_BlindUsage.cpp
 * Author: jake
 *
 * Created on September 28, 2013, 1:27 PM
 */
#include <cmath>
#include <algorithm>
#include "Utility.h"
#include "Model_ExternalShading.h"

Model_ExternalShading::Model_ExternalShading() {

  a01arr = -7.41;
  b01inarr = 0.001035;
  b01sarr = 2.17;
  // Probability of raising on arrival
  a10arr = -1.520;
  b10inarr = -0.000654;
  b10sarr = -3.139;
  // Probability of lowering during presence
  a01int = -8.013;
  b01inint = 0.000841;
  b01sint = 1.270;
  // Probability of raising during presence
  a10int = -3.625;
  b10inint = -0.000276;
  b10sint = -2.683;
  // Probability of full raising
  afullraise = 0.435;
  boutfullraise = 1.95;
  bsfullraise = -0.0000231;
  // Choice of new unshaded fraction
  aSFlower = -2.294;
  bSFlower = 1.522;
  shapelower = 1.708;

  bsfulllower = 0.00000091;
  boutfulllower = -2.23;
  afulllower = -0.27;

}
/*
void Model_ExternalShading::calculate(double state, double Lumint, double Evg, bool currentlyOccupied, bool previouslyOccupied) {
    // This function simulates the occurence of actions on lower shading devices (covering the
    // vision window). If an action is simulated, the chosen unshaded fraction is then predicted.
    // Input parameters are: indoor workplane illuminance (Lumint) and outdoor global horizontal
    // illuminance (Evg).
    // 1 is open
    // 0 is closed
    previousShadingState = state;
    //std::cout << "previous blind state: " << state << std::endl;

    // --- Simulation of lower blinds -------------------------------------------------------------

    if (!currentlyOccupied && !previouslyOccupied) {
        currentShadingState = previousShadingState;
    } else if (currentlyOccupied && !previouslyOccupied) {
        arrival(Lumint, Evg);
    } else {
        departure(Lumint, Evg);
    }
    return;
}
*/

void Model_ExternalShading::setFullVars(float afullraise, float boutfullraise, float bsfullraise, float bsfulllower, float boutfulllower, float afulllower){

  this->afullraise = afullraise;
  this->boutfullraise = boutfullraise;
  this->bsfullraise = bsfullraise;

  this->bsfulllower = bsfulllower;
  this->boutfulllower = boutfulllower;
  this->afulllower = afulllower;
}

void Model_ExternalShading::setDurationVars(float aSFlower, float bSFlower, float shapelower){
  this->aSFlower = aSFlower;
  this->bSFlower = bSFlower;
  this->shapelower = shapelower;
}

void Model_ExternalShading::setArrivalVars(float a01arr, float b01inarr, float b01sarr, float a10arr, float b10inarr, float b10sarr){
  this->a01arr = a01arr;
  this->b01inarr = b01inarr;
  this->b01sarr = b01sarr;
  // Probability of raising on arrival
  this->a10arr = a10arr;
  this->b10inarr = b10inarr;
  this->b10sarr = b10sarr;

}

void Model_ExternalShading::setInterVars(float a01int, float b01inint, float b01sint, float a10int, float b10inint, float b10sint){
  // Probability of lowering during presence
  this->a01int = a01int;
  this->b01inint = b01inint;
  this->b01sint = b01sint;
  // Probability of raising during presence
  this->a10int = a10int;
  this->b10inint = b10inint;
  this->b10sint = b10sint;
}


double Model_ExternalShading::arrival(double state, double Lumint, double Evg) {
    double currentShadingState;

    double probraise = 0.f;
    if (state != 1.f) {
        double m_raise = a10arr + b10inarr * Lumint + b10sarr * (state);
        probraise = probability(m_raise);
    }
    double problower = 0.f;
    if (state != 0.f) {
        double m_lower = a01arr + b01inarr * Lumint + b01sarr * (state);
        problower = probability(m_lower);
    }
    if (problower >= probraise) {
        if (randomDouble() < problower) {
            currentShadingState = arrivalLowering(state, Lumint, Evg);
        }
        else if (randomDouble() < probraise) {
            currentShadingState = arrivalRaising(state, Lumint, Evg);
        }
        else {
            currentShadingState = state;
        }
    }
    else {
        if (randomDouble() < probraise) {
            currentShadingState = arrivalRaising(state, Lumint, Evg);
        }
        else if (randomDouble() < problower) {
            currentShadingState = arrivalLowering(state, Lumint, Evg);
        }
        else {
            currentShadingState = state;
        }
    }
    return currentShadingState;
}


double Model_ExternalShading::intermediate(bool state, double Lumint, double Evg){
    return departure(state, Lumint, Evg);
}

double Model_ExternalShading::departure(double state, double Lumint, double Evg) {
    double currentShadingState;
    float problower = 0.f;
    float probraise = 0.f;

    if (state != 1.f) {
        double m_probraise = a10int + b10inint * Lumint + b10sint * (state);
        probraise = probability(m_probraise);
    }
    if (state != 0.f) {
        double m_problower = a01int + b01inint * Lumint + b01sint * (state);
        problower = probability(m_problower);
    }

    if (problower >= probraise) {
        if (randomDouble() < problower) {
            currentShadingState = departureLowering(state, Lumint, Evg);
        }
        else if (randomDouble() < probraise) {
            currentShadingState = departureRaising(state, Lumint, Evg);
        }
        else {
            currentShadingState = state;
        }
    }
    else {
        if (randomDouble() < probraise) {
            currentShadingState = departureRaising(state, Lumint, Evg);
        }
        else if (randomDouble() < problower) {
            currentShadingState = departureLowering(state, Lumint, Evg);
        }
        else {
            currentShadingState = state;
        }
    }
    return currentShadingState;
}

double Model_ExternalShading::arrivalRaising(double state, double Lumint, double Evg) {
    double currentShadingState;
    double m_totraise = afullraise + boutfullraise * Evg + bsfullraise * (state);
    float ptotraise = probability(m_totraise);
    double r = randomDouble();
    if (r < ptotraise) {
        currentShadingState = 1.f;
    }
    else {
        currentShadingState = 0.01f * round(100.f * randomDouble((state), 1.f));
    }
    return currentShadingState;
}

double Model_ExternalShading::arrivalLowering(double state, double Lumint, double Evg) {
    double currentShadingState;
    double m_totlow = afulllower + boutfulllower * Evg + bsfulllower * (state);
    float ptotlow = probability(m_totlow);
    if (randomDouble() < ptotlow) {
        currentShadingState = 0.f;
    }
    else {
        float Reduction = randomWeibull(exp(aSFlower + bSFlower * (state)), shapelower);
        currentShadingState = (0.01f * round(100.f * std::max((state) - Reduction, 0.01)));
    }
    return currentShadingState;
}

double Model_ExternalShading::departureLowering(double state, double Lumint, double Evg) {
    double currentShadingState;
    double m_ptotlow = afulllower + boutfulllower * Evg + bsfulllower * (state);
    float ptotlow = probability(m_ptotlow);
    if (randomDouble() < ptotlow) {
        currentShadingState = (0.f);
    }
    else {
        float Reduction = randomWeibull(exp(aSFlower + bSFlower * (state)), shapelower);
        currentShadingState = (0.01f * round(100.f * std::max((state) - Reduction, 0.01)));
    }
    return currentShadingState;
}

double Model_ExternalShading::departureRaising(double state, double Lumint, double Evg) {
    double currentShadingState;
    double m_totraise = afullraise + boutfullraise * Evg + bsfullraise * (state);
    float ptotraise = probability(m_totraise);

    if (randomDouble(0.f, 1.f) < ptotraise) {
        currentShadingState = (1.f);
    }
    else {
        currentShadingState = (0.01f * round(100.f * randomDouble((state), 1.f)));
    }
    return currentShadingState;
}

/*
 * File:   Model_BlindUsage.h
 * Author: jake
 *
 * Created on September 28, 2013, 1:27 PM
 */

#ifndef MODEL_BLINDUSAGE_H
#define	MODEL_BLINDUSAGE_H

class Model_ExternalShading {
public:
    Model_ExternalShading();
    //void calculate(double state, double Lumint, double Evg, bool currentlyOccupied, bool previouslyOccupied);
    double arrival(double state, double Lumint, double Evg);
    double departure(double state, double Lumint, double Evg);
    double intermediate(bool state, double Lumint, double Evg);

    void setFullVars(float afullraise, float boutfullraise, float bsfullraise, float bsfulllower, float boutfulllower, float afulllower);
    void setDurationVars(float aSFlower, float bSFlower, float shapelower);
    void setArrivalVars(float a01arr, float b01inarr, float b01sarr, float a10arr, float b10inarr, float b10sarr);
    void setInterVars(float a01int, float b01inint, float b01sint, float a10int, float b10inint, float b10sint);


private:
    double arrivalRaising(double state, double Lumint, double Evg);
    double arrivalLowering(double state,double Lumint, double Evg);
    double departureLowering(double state,double Lumint, double Evg);
    double departureRaising(double state,double Lumint, double Evg);
    // Probability of lowering on arrival
    float a01arr;
    float b01inarr;
    float b01sarr;
    // Probability of raising on arrival
    float a10arr;
    float b10inarr;
    float b10sarr;
    // Probability of lowering during presence
    float a01int;
    float b01inint;
    float b01sint;
    // Probability of raising during presence
    float a10int;
    float b10inint;
    float b10sint;
    // Probability of full raising
    float afullraise;
    float boutfullraise;
    float bsfullraise;
    // Choice of new unshaded fraction
    float aSFlower;
    float bSFlower;
    float shapelower;

    float bsfulllower;
    float boutfulllower;
    float afulllower;


};

#endif	/* MODEL_BLINDUSAGE_H */

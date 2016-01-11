/*
 * File:   Model_Presence.h
 * Author: jake
 *
 * Created on September 17, 2013, 3:13 PM
 */

#ifndef MODEL_PRESENCE_H
#define	MODEL_PRESENCE_H
#include <vector>

#include "Model_Activity.h"

class Model_Presence {
public:

    static std::vector<int> calculatePresenceFromActivities(const std::vector<double> activities) ;
    static std::vector<int> calculatePresenceFromPage(const int agentID);
private:
    Model_Presence();

    static double getT11(double pcurr, double pnext, double shuff) ;
    static double getT01(double pcurr, double pnext, double shuff) ;

    static int calculateNumberOfDays(int startDay, int startMonth, int endDay, int endMonth) ;

};

#endif	/* MODEL_PRESENCE_H */

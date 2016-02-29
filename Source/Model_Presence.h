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
    Model_Presence();
    void calculatePresenceFromActivities(const std::vector<double> activities);
    void calculatePresenceFromPage(const int agentID);
    unsigned int size() const;
    bool at(const int i) const;
    int presentForFutureSteps() const;

private:
    //std::vector<int> presenceForFutureSteps;
    //std::vector<int> currentDurationOfPresenceState;
    std::vector<int> presenceState;
    double getT11(double pcurr, double pnext, double shuff);
    double getT01(double pcurr, double pnext, double shuff);

    static int calculateNumberOfDays(int startDay, int startMonth, int endDay, int endMonth) ;

};

#endif	/* MODEL_PRESENCE_H */

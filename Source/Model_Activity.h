/*
 * File:   Model_Activity.h
 * Author: jake
 *
 * Created on September 17, 2013, 3:13 PM
 */

#ifndef MODEL_ACTIVITY_H
#define	MODEL_ACTIVITY_H

#include <vector>

class Model_Activity {
public:
    Model_Activity();
    static std::vector<double> getAgentActivities(int agentID);

private:
};

#endif	/* MODEL_ACTIVITY_H */

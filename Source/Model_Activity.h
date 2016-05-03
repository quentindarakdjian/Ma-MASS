/*
 * File:   Model_Activity.h
 * Author: jake
 *
 * Created on September 17, 2013, 3:13 PM
 */

#ifndef MODEL_ACTIVITY_H
#define	MODEL_ACTIVITY_H

#include <vector>
#include "SimulationConfig.h"

class Model_Activity {
public:
    Model_Activity();
    std::vector<double> preProcessActivities(int agentID);
    void multinominalP(double p[4][7][24][10], const int agentID) const;
    int multinominalActivity (const double *p) const;

private:
    void parseConfiguration(const std::string filename);
    std::vector<double> disaggregate(const int agentID) const;
    std::vector<double> multinominal(const int agentID) const;
    std::string getSeasonString(const int month) const;
    std::string getDay(const int day) const;
    int getSeasonInt(const int month)const;
    std::map<int, std::map<std::string, std::vector<double>>> dictionary;
    std::map<int, std::map<std::string, std::vector<double>>> dictLenght;
};

#endif	/* MODEL_ACTIVITY_H */

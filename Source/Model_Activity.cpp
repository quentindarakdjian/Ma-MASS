/*
 * File:   Model_Activity.cpp
 * Author: jake
 *
 * Created on September 17, 2013, 3:13 PM
 */

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "SimulationConfig.h"
#include "Utility.h"
#include <vector>
#include <cctype>
#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include "Model_Activity.h"

/**
 * sleep
 * passive
 * Audio / visual
 * IT
 * Cooking
 * Cleaning
 * Other Washing
 * Metabolic
 * Washing applicance
 * out
 */

Model_Activity::Model_Activity() {
}

std::vector<double> Model_Activity::getAgentActivities(int agentID) {

    double probabilities[24][10];
    std::map<int, std::string> probMap = SimulationConfig::agents.at(agentID).profile;
    for(int hour = 0; hour < 24; hour++) {
        std::vector<std::string> tokProbs;
        boost::split(tokProbs, probMap.at(hour), boost::is_any_of(","));
        int activity = 0;
        for(std::string strProb: tokProbs) {
            probabilities[hour][activity] = boost::lexical_cast<double>(strProb);
            activity++;
        }
    }

    std::vector<double> activities;
    int tsph = SimulationConfig::info.timeStepsPerHour;
    int hourCount = -1;
    for (int i = 0; i <= SimulationConfig::info.timeSteps; i++) {
        if(i % tsph == 0 || hourCount < 0){
            hourCount++;
            if(hourCount == 24){
                hourCount = 0;
            }
        }
        double drand = Utility::randomDouble(0.0,1.0);
        double totalProb = 0;
        for(int j = 0; j < 10; j++){
            totalProb += probabilities[hourCount][j];
            if(totalProb >= drand){
                activities.push_back(j);
                break;
            }
        }
    }
    return activities;
}


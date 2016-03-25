
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <map>
#include <limits>
#include <utility>
#include <vector>
#include <cctype>
#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include <cassert>

#include "Model_Activity.h"
#include "SimulationConfig.h"
#include "Utility.h"

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

std::vector<double> Model_Activity::preProcessActivities(int agentID){
    if (SimulationConfig::ActivityFile == ""){
        return disaggregate(agentID);
    }
    else{
        parseConfiguration(SimulationConfig::ActivityFile);
        return multinominal(agentID);
    }
}

std::vector<double> Model_Activity::disaggregate(const int agentID) const{
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

void Model_Activity::parseConfiguration(const std::string filename){
    namespace bpt = boost::property_tree;
    // Create an empty property tree object
    bpt::ptree pt;
    // Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.
    bpt::read_xml(filename, pt);
    // Iterate over the debug.modules section and store all found
    // modules in the m_modules set. The get_child() function
    // returns a reference to the child at the specified path; if
    // there is no such child, it throws. Property tree iterators
    // are models of BidirectionalIterator.
    for (bpt::ptree::value_type & child : pt.get_child("Activity")) { //For each hour of the activity file (1,7, 8, ..., 24)
        std::string inter = child.first;
        inter.erase(0, 8);
        int hour = boost::lexical_cast<int>(inter);
        std::map<std::string, std::vector<double>> items; // map de (name + tokProbsD)
        for (bpt::ptree::value_type & childchild : child.second) { // For each line of the treated hour
            std::string name = childchild.first; //name exemple "civstat1", "sex2", ..
            std::string cofList = childchild.second.get_value<std::string>(); //cofList exemple "0.234,0.018,-0.051,-0.88,-0.001,0.522,0.176,0.113,0.833"
            std::vector<std::string> tokProbs; // Empty vector
            std::vector<double> tokProbsD;
            boost::split(tokProbs, cofList, boost::is_any_of(",")); //Split cofList eactheh time there is a coma and fill the vector of string tokProbs with each element
            for (std::string strProb : tokProbs) {
                tokProbsD.push_back(boost::lexical_cast<double>(strProb)); //Each string of tokProbs is transform to double in the vector of double tokProbsD
            }
            std::pair<std::string, std::vector<double>> x(name, tokProbsD); //Creation of pairs of name (civstat1) and associated vector of double tokProbsD
            items.insert(x); //Fill the map items with all pair x
        }
        std::pair<int, std::map<std::string, std::vector<double>>> y(hour, items); //Create a pair y with hours and the map item
        dictionary.insert(y); // Filling the map dictionnary with all y
    }
}

std::vector<double> Model_Activity::multinominal(const int agentID) const {
    double p[4][7][24][10];
    multinominalP(p, agentID);

    std::vector<double> activities;

    static const int daysInMonth[] =
      {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int tsph = SimulationConfig::info.timeStepsPerHour;
    int hourCount = 0;
    int month = SimulationConfig::info.startMonth -1;
    int day = SimulationConfig::info.startDay -1;
    int dayOfWeek = SimulationConfig::info.startDayOfWeek-1;

    int season = getSeasonInt(month);

    for (int i = 0; i <= SimulationConfig::info.timeSteps; i++) {
        if (i % tsph == 0 || hourCount < 1) {
            hourCount++;
            if (hourCount > 24) {
                hourCount = 1;
                day++;
                dayOfWeek++;
                if (dayOfWeek > 6) {
                  dayOfWeek = 0;
                }
                if (day > daysInMonth[month]) {
                  month++;
                  day = 1;
                  if (month == 13) {
                    month = 1;
                  }
                  season = getSeasonInt(month);
                }
            }
        }
        activities.push_back(multinominalActivity(p[season][dayOfWeek][hourCount]));
    }
    return activities;
}

void Model_Activity::multinominalP(double p[4][7][24][10], const int agentID) const {

    int ageCat = 0;
    int ageInt = SimulationConfig::agents.at(agentID).age;
    if (ageInt <= 40){ageCat = 3;}
    else if (ageInt > 40 && ageInt <= 63){ageCat = 2;}
    else {ageCat = 1;}
    std::string age = "age"+std::to_string(ageCat);
    std::string computer = SimulationConfig::agents.at(agentID).computer;
    std::string civstat = SimulationConfig::agents.at(agentID).civstat;
    std::string unemp = SimulationConfig::agents.at(agentID).unemp;
    std::string retired = SimulationConfig::agents.at(agentID).retired;
    std::string edtry = SimulationConfig::agents.at(agentID).edtry;
    std::string famstat = SimulationConfig::agents.at(agentID).famstat;
    std::string sex = SimulationConfig::agents.at(agentID).sex;

    for (int iSeason = 0; iSeason <4; iSeason++) {
      std::string seasonString = getSeasonString(iSeason);
      for (int iDay = 0; iDay <7; iDay++) {
        std::string dayString = getDay(iDay+1);
        for (int iHour = 0; iHour <24; iHour++) {
          double g[10];
          double d = 0;
          int hour = iHour +1;
          if (iHour < 6) {
            hour = 1;
          }

          for (int i =0; i < 9; i++) {
            g[i] = dictionary.at(hour).at("Intercept").at(i) +
                   dictionary.at(hour).at(age).at(i) +
                   dictionary.at(hour).at(seasonString).at(i) +
                   dictionary.at(hour).at(computer).at(i) +
                   dictionary.at(hour).at(civstat).at(i) +
                   dictionary.at(hour).at(unemp).at(i) +
                   dictionary.at(hour).at(retired).at(i) +
                   dictionary.at(hour).at(edtry).at(i) +
                   dictionary.at(hour).at(famstat).at(i) +
                   dictionary.at(hour).at(dayString).at(i) +
                   dictionary.at(hour).at(sex).at(i);
            d += std::exp(g[i]);
          }
          g[9] = 0;
          d += 1;  // == std::exp(g[9]) == exp(0) == 1;
          double sum = 0;
          for (int i =0; i < 10; i++) {
            p[iSeason][iDay][iHour][i] = std::exp(g[i]) / d;
            sum += p[iSeason][iDay][iHour][i];
          }
          assert(std::abs(1.0-sum) < std::numeric_limits<double>::epsilon() * std::abs(1.0+sum)* 2);
        }
      }
    }
}

std::string Model_Activity::getSeasonString(const int month) const{
    std::string season;
    switch (month){
        case 0:
            season = "season1";
            break;
        case 1:
            season = "season2";
            break;
        case 2:
            season = "season3";
            break;
        default:
            season = "season4";
    }
    return season;
}

std::string Model_Activity::getDay(const int day) const{
    return "day" + std::to_string(day);
}

int Model_Activity::getSeasonInt(const int month)const{
    int season;
    switch(month){
    case 12:
    case 1:
    case 2:
        season = 3;
        break;
    case 3:
    case 4:
    case 5:
        season = 0;
        break;
    case 6:
    case 7:
    case 8:
        season = 1;
        break;
    default:
        season = 2;
    }
    return season;
}

int Model_Activity::multinominalActivity(const double *p)const{
    int activity;
    double sum = 0;
    double drand = Utility::randomDouble(0.0, 1.0);
    for (int i = 0; i < 10; i++){
        sum += p[i];
        if (sum >= drand){
            activity = i;
            break;
        }
    }
    return activity;
}



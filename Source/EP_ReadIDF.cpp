// Copyright AI Environnement 2017

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include "SimulationConfig.h"
#include "Zone.h"
#include "EP_IDFObject.h"
#include "EP_ReadIDF.h"

std::string choppa(const std::string &t, const std::string &ws) {
        std::string str = t;
        size_t found;
        found = str.find_last_not_of(ws);
        if (found != std::string::npos)
                str.erase(found + 1);
        else
                str.clear();  // str is all whitespace

        found = str.find_first_not_of(ws);
        if (found < str.size()) {

                str.replace(0, found, "");

        }
        return str;
}

EP_ReadIDF::EP_ReadIDF() {
        numberOfTimeSteps = -1;
}

bool EP_ReadIDF::processLine(std::stringstream *ss, std::string line) {
        bool ret = false;
        boost::algorithm::erase_all(line, "\r");
        if (line.size() < 1 || line.empty() || line.compare("") == 0) {
                return ret;
        }
        std::vector<std::string> strs;
        boost::split(strs, line, boost::is_any_of("!"));
        std::string ssss = choppa(strs[0], " ");
        std::vector<std::string> strs1;
        boost::split(strs1, ssss, boost::is_any_of(","));
        std::stringstream splitStringStream;
        for (unsigned int i = 0; i < strs1.size(); i++) {
                if (i > 0) {
                        splitStringStream << ",";
                }
                splitStringStream << choppa(strs1[i], " ");
        }
        ssss = splitStringStream.str();
        if (ssss.size() < 1) {
                return ret;
        }
        if (boost::algorithm::contains(ssss, ";")) {
                boost::algorithm::erase_all(ssss, ";");
                ret = true;
        }
        (*ss) << (ssss);
        return ret;
}

int EP_ReadIDF::getOrCreateZone(std::string name) {
        int i = 0;

        for(Zone o : zones) {
                if (o.getName() == name) {
                        return i;
                }
                i++;
        }

        Zone z(name);
        zones.push_back(z);
        return i;
}

void EP_ReadIDF::loadIDF() {
        std::ifstream file("in.idf", std::ios_base::in | std::ios_base::binary);
        if (file.is_open()) {
                boost::iostreams::filtering_istream in;
                in.push(file);
                std::stringstream ss;
                std::string line;
                while (getline(in, line)) {
                        if (processLine(&ss, line)) {
                                idfObjs.push_back(EP_IDFObject(ss.str()));
                                ss.str("");
                        }
                }
        }
        for(EP_IDFObject o : idfObjs) {
                if (o.is("Zone") || o.is("ZONE") ) {
                        std::string name = o.at(1);
                        if (SimulationConfig::zones.find(name) != SimulationConfig::zones.end()) {
                                getOrCreateZone(name);
                                continue;
                        }
                }
                if (o.is("Timestep") || o.is("TIMESTEP")) {
                        timeStepPerHour = boost::lexical_cast<int>(o.at(1));
                        continue;
                }
                if (o.is("RunPeriod") || o.is("RUNPERIOD")) {
                        beginMonth = boost::lexical_cast<int>(o.at(2));
                        endMonth = boost::lexical_cast<int>(o.at(4));
                        beginDay = boost::lexical_cast<int>(o.at(3));
                        endDay = boost::lexical_cast<int>(o.at(5));
                        SimulationConfig::info.timePeriod++;
                        continue;
                }
                if (o.is("SizingPeriod:DesignDay") || o.is("SIZINGPERIOD:DESIGNDAY")) {
                        SimulationConfig::info.timePeriod++;
                        continue;
                }
        }
}

int EP_ReadIDF::timeSteps() {
        if (numberOfTimeSteps < 0) {
                int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
                int days = daysInMonth[beginMonth - 1] - (beginDay - 1);
                for (int i = beginMonth; i < endMonth -1; i++) {
                        days += daysInMonth[i];
                }
                if (beginMonth != endMonth) {
                        days += endDay;
                }

                int hours = days * 24;
                numberOfTimeSteps = hours * timeStepPerHour;
        }
        return numberOfTimeSteps;
}

int EP_ReadIDF::timeStepsPerHour() {
        return timeStepPerHour;
}

std::vector<std::string> EP_ReadIDF::getZoneNames() const {
        std::vector<std::string> names;

        for(Zone o : zones) {
                names.push_back(o.getName());
        }
        return names;
}

std::vector<Zone> EP_ReadIDF::getZones() const {
        return zones;
}

int EP_ReadIDF::getEndDay() const {
        return endDay;
}

int EP_ReadIDF::getBeginDay() const {
        return beginDay;
}

int EP_ReadIDF::getEndMonth() const {
        return endMonth;
}

int EP_ReadIDF::getBeginMonth() const {
        return beginMonth;
}

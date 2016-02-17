/*
 * File:   SimulationSetup.cpp
 * Author: jake
 *
 * Created on September 13, 2013, 10:17 AM
 */


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <map>
#include "SimulationConfig.h"
#include "Utility.h"

std::map<std::string, zoneStruct> SimulationConfig::zones;
std::map<int, windowStruct> SimulationConfig::windows;
std::map<int, shadeStruct> SimulationConfig::shades;
std::vector<agentStruct> SimulationConfig::agents;
std::vector<stateStruct> SimulationConfig::states;
buildingStruct SimulationConfig::building;
simulationStruct SimulationConfig::info;
int SimulationConfig::stepCount = -1;

SimulationConfig::SimulationConfig(){
}

void SimulationConfig::parseBuilding(boost::property_tree::ptree::value_type & v){
    for(boost::property_tree::ptree::value_type & child: v.second){
        if (child.first == "globalBuilding"){
            std::pair<std::string, buildingStruct> building;
            for(boost::property_tree::ptree::value_type & childschild: child.second){
                if (childschild.first == "localisation"){
                    SimulationConfig::building.localisation = childschild.second.data();
                }
                else if (childschild.first == "roomThermostat"){
                    SimulationConfig::building.roomThermostat = childschild.second.get_value<bool>();
                }
                else if (childschild.first == "thermostaticRadiatorValve"){
                    SimulationConfig::building.thermostaticRadiatorValve = childschild.second.get_value<bool>();
                }
                else if (childschild.first == "houseHoldIncome"){
                    SimulationConfig::building.houseHoldIncome = childschild.second.get_value<double>();
                }
                else if (childschild.first == "tenureType"){
                    SimulationConfig::building.tenureType = childschild.second.data();
                }
                else if (childschild.first == "typology"){
                    SimulationConfig::building.typology = childschild.second.data();
                }
                else if (childschild.first == "mainFuel"){
                    SimulationConfig::building.mainFuel = childschild.second.data();
                }
                else if (childschild.first == "additionalFuel"){
                    SimulationConfig::building.additionalFuel = childschild.second.data();
                }
                else if (childschild.first == "yearOfConstruction"){
                    SimulationConfig::building.yearOfConstruction = childschild.second.get_value<int>();
                }
                else if (childschild.first == "roofInsulationThickness"){
                    SimulationConfig::building.roofInsulationThickness = childschild.second.get_value<double>();
                }
                else if (childschild.first == "extendOfDoubleGlazing"){
                    SimulationConfig::building.extendOfDoubleGlazing = childschild.second.data();
                }
                else if (childschild.first == "wallUValue"){
                    SimulationConfig::building.wallUValue = childschild.second.get_value<double>();
                }
            }
        }
    }
    for(boost::property_tree::ptree::value_type & child: v.second){
        if (child.first == "zone"){
            std::pair<std::string, zoneStruct> zone;
            for(boost::property_tree::ptree::value_type & childschild: child.second){
                if (childschild.first == "name"){
                    //std::cout << "name: " << childschild.second.data() << std::endl;
                    zone.first = childschild.second.data();
                    zone.second.name = childschild.second.data();
                }
                else if (childschild.first == "activities" || childschild.first == "activity"){
                    //std::cout << "type: " << childschild.second.data() << std::endl;
                    zone.second.activity = childschild.second.data();
                }
                else if (childschild.first == "groundFloor"){
                    //std::cout << "type: " << childschild.second.data() << std::endl;
                    zone.second.groundFloor = childschild.second.get_value<bool>();
                }
            }
            std::cout << "Name: " << zone.first << " Activity: " << zone.second.activity;
            std::cout << " Floor: " << zone.second.groundFloor << std::endl;
            if(zone.second.activity == ""){
                std::cout << "Did not define activities for zone: " << zone.second.name << std::endl;
                exit(-1);
            }
            zones.insert(zone);
        }
    }
}

void SimulationConfig::parseAgents(boost::property_tree::ptree::value_type & v){
    for(boost::property_tree::ptree::value_type & child: v.second){
        if (child.first == "agent"){
            agentStruct agent;
            for(boost::property_tree::ptree::value_type & childschild: child.second){
                if (childschild.first == "profile"){
                    for(boost::property_tree::ptree::value_type & childschildchild: childschild.second){
                        std::string text = childschildchild.first;
                        if(SimulationConfig::info.presencePage){
                            std::pair<int, std::string> a;
                            if(text == "monday"){
                                a.first = 0;
                            }else if(text == "tuesday"){
                                a.first = 1;
                            }else  if(text == "wednesday"){
                                a.first = 2;
                            }else  if(text == "thursday"){
                                a.first = 3;
                            }else  if(text == "friday"){
                                a.first = 4;
                            }else  if(text == "saturday"){
                                a.first = 5;
                            }else  if(text == "sunday"){
                                a.first = 6;
                            }
                            a.second = childschildchild.second.get_value<std::string>();
                            agent.profile.insert(a);
                        }
                        else{
                            text.erase(0,1);
                            std::pair<int, std::string> a;
                            a.first = boost::lexical_cast<int>(text);
                            a.second = childschildchild.second.get_value<std::string>();
                            agent.profile.insert(a);
                        }
                    }
                }
                else if(childschild.first == "bedroom"){
                    agent.bedroom = childschild.second.data();
                }
                else if(childschild.first == "office"){
                    agent.office = childschild.second.data();
                }
                else if(childschild.first == "power"){
                    agent.power = childschild.second.get_value<double>();
                }
                else if(childschild.first == "age"){
                    agent.age = childschild.second.get_value<int>();
                }
                else if(childschild.first == "window"){
                    agent.windowId = childschild.second.get_value<int>();
                }
                else if(childschild.first == "shade"){
                    agent.shadeId = childschild.second.get_value<int>();
                }
            }
            agents.push_back(agent);
        }
    }
}


void SimulationConfig::parseStates(boost::property_tree::ptree::value_type & v){
    for(boost::property_tree::ptree::value_type & child: v.second){
        if (child.first == "state"){
            stateStruct state;
            for(boost::property_tree::ptree::value_type & childschild: child.second){
                if (childschild.first == "name"){
                    state.name = childschild.second.data();
                }
                else if(childschild.first == "location"){
                    state.location = childschild.second.data();
                }
                else if(childschild.first == "clo"){
                    state.clo = childschild.second.get_value<int>();
                }
                else if(childschild.first == "metabolicRate"){
                    state.met = childschild.second.get_value<int>();
                }
            }
            states.push_back(state);
        }
    }
}

void SimulationConfig::parseModels(boost::property_tree::ptree::value_type & v){
    for(boost::property_tree::ptree::value_type & child: v.second){
        if (child.first == "windows"){
            parseWindows(child);
        }
        else if (child.first == "shades"){
            parseShades(child);
        }
        else if (child.first == "lights"){
            SimulationConfig::info.lights = child.second.get_value<bool>();
        }
        else if (child.first == "heatingSetpoint"){
            SimulationConfig::info.heatingSetpoint = child.second.get_value<bool>();
        }
    }
}

void SimulationConfig::parseWindows(boost::property_tree::ptree::value_type & v){
    for(boost::property_tree::ptree::value_type & child: v.second){
        if (child.first == "enabled"){
            SimulationConfig::info.windows = child.second.get_value<bool>();
        }
        else if (child.first == "window"){
            std::pair<int, windowStruct> ws;
            for(boost::property_tree::ptree::value_type & childschild: child.second){
                if (childschild.first == "id"){
                    ws.first = childschild.second.get_value<int>();
                }
                else if (childschild.first == "aop"){
                    ws.second.aop = childschild.second.get_value<double>();
                }
                else if (childschild.first == "bopout"){
                    ws.second.bopout = childschild.second.get_value<double>();
                }
                else if (childschild.first == "shapeop"){
                    ws.second.shapeop = childschild.second.get_value<double>();
                }
                else if (childschild.first == "a01arr"){
                    ws.second.a01arr = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01inarr"){
                    ws.second.b01inarr = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01outarr"){
                    ws.second.b01outarr = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01absprevarr"){
                    ws.second.b01absprevarr = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01rnarr"){
                    ws.second.b01rnarr = childschild.second.get_value<double>();
                }
                else if (childschild.first == "a01int"){
                    ws.second.a01int = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01inint"){
                    ws.second.b01inint = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01outint"){
                    ws.second.b01outint = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01presint"){
                    ws.second.b01presint = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01rnint"){
                    ws.second.b01rnint = childschild.second.get_value<double>();
                }
                else if (childschild.first == "a01dep"){
                    ws.second.a01dep = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01outdep"){
                    ws.second.b01outdep = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01absdep"){
                    ws.second.b01absdep = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01gddep"){
                    ws.second.b01gddep = childschild.second.get_value<double>();
                }
                else if (childschild.first == "a10dep"){
                    ws.second.a10dep = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b10indep"){
                    ws.second.b10indep = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b10outdep"){
                    ws.second.b10outdep = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b10absdep"){
                    ws.second.b10absdep = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b10gddep"){
                    ws.second.b10gddep = childschild.second.get_value<double>();
                }
            }
            windows.insert(ws);
        }
    }
}

void SimulationConfig::parseShades(boost::property_tree::ptree::value_type & v){
    for(boost::property_tree::ptree::value_type & child: v.second){
        if (child.first == "enabled"){
            SimulationConfig::info.shading = child.second.get_value<bool>();
        }
        else if (child.first == "shade"){
            std::pair<int, shadeStruct> ws;

            for(boost::property_tree::ptree::value_type & childschild: child.second){
                if (childschild.first == "id"){
                    ws.first = childschild.second.get_value<int>();
                }
                else if (childschild.first == "a01arr"){
                    ws.second.a01arr = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01inarr"){
                    ws.second.b01inarr = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01sarr"){
                    ws.second.b01sarr = childschild.second.get_value<double>();
                }
                else if (childschild.first == "a10arr"){
                    ws.second.a10arr = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b10inarr"){
                    ws.second.b10inarr = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b10sarr"){
                    ws.second.b10sarr = childschild.second.get_value<double>();
                }
                else if (childschild.first == "a01int"){
                    ws.second.a01int = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01inint"){
                    ws.second.b01inint = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b01sint"){
                    ws.second.b01sint = childschild.second.get_value<double>();
                }
                else if (childschild.first == "a10int"){
                    ws.second.a10int = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b10inint"){
                    ws.second.b10inint = childschild.second.get_value<double>();
                }
                else if (childschild.first == "b10sint"){
                    ws.second.b10sint = childschild.second.get_value<double>();
                }
                else if (childschild.first == "afullraise"){
                    ws.second.afullraise = childschild.second.get_value<double>();
                }
                else if (childschild.first == "boutfullraise"){
                    ws.second.boutfullraise = childschild.second.get_value<double>();
                }
                else if (childschild.first == "bsfullraise"){
                    ws.second.bsfullraise = childschild.second.get_value<double>();
                }
                else if (childschild.first == "bsfulllower"){
                    ws.second.bsfulllower = childschild.second.get_value<double>();
                }
                else if (childschild.first == "boutfulllower"){
                    ws.second.boutfulllower = childschild.second.get_value<double>();
                }
                else if (childschild.first == "afulllower"){
                    ws.second.afulllower = childschild.second.get_value<double>();
                }
                else if (childschild.first == "aSFlower"){
                    ws.second.aSFlower = childschild.second.get_value<double>();
                }
                else if (childschild.first == "bSFlower"){
                    ws.second.bSFlower = childschild.second.get_value<double>();
                }
                else if (childschild.first == "shapelower"){
                    ws.second.shapelower = childschild.second.get_value<double>();
                }
            }
            shades.insert(ws);
        }
    }
}

/**
 * @brief Parses the simulation configuration file
 * @details Parse the configuration file for the all the parameteres needed in the simulation
 *
 * @param filename location of the simulation file to parse.
 */
void SimulationConfig::parseConfiguration(std::string filename){

    // Create an empty property tree object
    boost::property_tree::ptree pt;
    // Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.

    boost::property_tree::read_xml(filename, pt);
    // Iterate over the debug.modules section and store all found
    // modules in the m_modules set. The get_child() function
    // returns a reference to the child at the specified path; if
    // there is no such child, it throws. Property tree iterators
    // are models of BidirectionalIterator.

    info.windows = false;
    info.shading = false;
    info.lights = false;
    info.heatingSetpoint = false;

    for(boost::property_tree::ptree::value_type & v: pt.get_child("simulation")){
        if (v.first == "seed"){
            Utility::setSeed(v.second.get_value<int>());
        }
        else if (v.first == "simulateAgents"){
            SimulationConfig::info.simulateAgents = v.second.get_value<bool>();
        }
        else if (v.first == "agents"){
            parseAgents(v);
        }
        else if (v.first == "states"){
            parseStates(v);
        }
        else if (v.first == "presencePage"){
            SimulationConfig::info.presencePage = v.second.get_value<bool>();
        }
        else if (v.first == "social"){
            SimulationConfig::info.social = v.second.get_value<bool>();
        }
        else if (v.first == "building"){
            parseBuilding(v);
        }
        else if (v.first == "models"){
            parseModels(v);
        }
    }
    std::cout << "Loaded XML file: -" << filename << "-" << std::endl;
}

std::string SimulationConfig::getZoneNameFromActivity(std::string activity){
    std::string zoneName = "Out"; //Default out of zone
    bool found = false;
    for (std::pair<std::string, zoneStruct> it : zones){
        std::vector<std::string> activities = splitZoneActivities(it.second.activity);
        for(std::string eachActivity: activities){
            if (eachActivity == activity)
            {
                zoneName = it.first;
                found = true;
                break;
            }
        }
        if(found){
            break;
        }
    }
    return zoneName;
}

std::vector<std::string> SimulationConfig::splitZoneActivities(std::string typeString){
    std::vector<std::string> types;
    const char *str = typeString.c_str();
    char c = ',';
    do{
        const char *begin = str;
        while (*str != c && *str)
        {
            str++;
        }
        types.push_back(std::string(begin, str));
        //std::cout << std::string(begin, str) << std::endl;
    }
    while (0 != *str++);
    return types;
}

std::vector<std::string> SimulationConfig::getActivities(std::string* name){
    std::vector<std::string> activities;
    if (activeZone(name)){
        zoneStruct zone = SimulationConfig::zones.at(*name);
        activities = splitZoneActivities(zone.activity);
    }
    return activities;
}

bool SimulationConfig::activeZone(std::string* zoneName){
    return (SimulationConfig::zones.find(*zoneName) != SimulationConfig::zones.end());
}

bool SimulationConfig::isZoneGroundFloor(std::string* zoneName){
    return SimulationConfig::zones.at(*zoneName).groundFloor;
}

int SimulationConfig::numberOfAgents(){
    return SimulationConfig::agents.size();
}

double SimulationConfig::lengthOfTimestep(){
    return 60.0 / SimulationConfig::info.timeStepsPerHour;
}

void SimulationConfig::step(){
    stepCount++;
}

int SimulationConfig::getStepCount(){
    return stepCount;
}

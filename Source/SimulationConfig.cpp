
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <map>
#include <utility>
#include <string>
#include <vector>
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
std::string SimulationConfig::ActivityFile;
std::string SimulationConfig::FmuLocation;

namespace bpt = boost::property_tree;

SimulationConfig::SimulationConfig(){
}

/*void SimulationConfig::reset(){
    agents.clear();
    zones.clear();
    windows.clear();
    shades.clear();
    info = simulationStruct();
    stepCount = -1;
    ActivityFile = "";
    FmuLocation = "";
}*/

void SimulationConfig::parseBuilding(bpt::ptree::value_type & v){
    zones.clear();
    for(bpt::ptree::value_type & child: v.second){
        if (child.first == "globalBuilding"){
            std::pair<std::string, buildingStruct> building;
            for(bpt::ptree::value_type & childschild: child.second){
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
    for(bpt::ptree::value_type & child: v.second){
        if (child.first == "zone"){
            std::pair<std::string, zoneStruct> zone;
            for(bpt::ptree::value_type & childschild: child.second){
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
            zones.insert(zone); // zones is a map
        }
    }
}

void SimulationConfig::parseAgents(bpt::ptree::value_type & v){
    agents.clear();
    SimulationConfig::ActivityFile = "";
    for(bpt::ptree::value_type & child: v.second){
        if (child.first == "agent"){
            agentStruct agent;
            for(bpt::ptree::value_type & schild: child.second){
                if (schild.first == "profile"){
                    for(bpt::ptree::value_type & sschild: schild.second){
                        std::string text = sschild.first;
                        if (text == "file"){
                            SimulationConfig::ActivityFile = sschild.second.get_value<std::string> ();
                        }
                        else {
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
                            }else{
                                text.erase(0,1);
                                a.first = boost::lexical_cast<int>(text);
                            }
                            a.second = sschild.second.get_value<std::string>();
                            agent.profile.insert(a);
                        }
                    }
                }
                else if(schild.first == "jobType"){
                    agent.jobType = schild.second.data();
                }
                else if(schild.first == "bedroom"){
                    agent.bedroom = schild.second.data();
                }
                else if(schild.first == "office"){
                    agent.office = schild.second.data();
                }
                else if(schild.first == "power"){
                    agent.power = schild.second.get_value<double>();
                }
                else if(schild.first == "age"){
                    agent.age = schild.second.get_value<int>();
                }
                else if (schild.first == "edtry") {
                    agent.edtry = schild.second.data();
                }
                else if (schild.first == "computer") {
                    agent.computer = schild.second.data();
                }
                else if (schild.first == "civstat") {
                    agent.civstat = schild.second.data();
                }
                else if (schild.first == "unemp") {
                    agent.unemp = schild.second.data();
                }
                else if (schild.first == "retired") {
                    agent.retired = schild.second.data();
                }
                else if (schild.first == "sex") {
                    agent.sex = schild.second.data();
                }
                else if (schild.first == "famstat") {
                    agent.famstat = schild.second.data();
                }
                else if(schild.first == "window"){
                    agent.windowId = schild.second.get_value<int>();
                }
                else if(schild.first == "shade"){
                    agent.shadeId = schild.second.get_value<int>();
                }
            }
            agents.push_back(agent);
        }
    }
}

void SimulationConfig::parseStates(bpt::ptree::value_type & v){
    for(bpt::ptree::value_type & child: v.second){
        if (child.first == "state"){
            stateStruct state;
            for(bpt::ptree::value_type & childschild: child.second){
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

void SimulationConfig::parseModels(bpt::ptree::value_type & v){
    for(bpt::ptree::value_type & child: v.second){
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

void SimulationConfig::parseWindows(bpt::ptree::value_type & v){
    windows.clear();
    for(bpt::ptree::value_type & child: v.second){
        if (child.first == "enabled"){
            SimulationConfig::info.windows = child.second.get_value<bool>();
        }
        else if (child.first == "window"){
            std::pair<int, windowStruct> ws;
            for(bpt::ptree::value_type & childschild: child.second){
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

void SimulationConfig::parseShades(bpt::ptree::value_type & v){
    shades.clear();
    for(bpt::ptree::value_type & child: v.second){
        if (child.first == "enabled"){
            SimulationConfig::info.shading = child.second.get_value<bool>();
        }
        else if (child.first == "shade"){
            std::pair<int, shadeStruct> ws;

            for(bpt::ptree::value_type & childschild: child.second){
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
    bpt::ptree pt;
    // Load the XML file into the property tree. If reading fails
    // (cannot open file, parse error), an exception is thrown.

    bpt::read_xml(filename, pt);
    // Iterate over the debug.modules section and store all found
    // modules in the m_modules set. The get_child() function
    // returns a reference to the child at the specified path; if
    // there is no such child, it throws. Property tree iterators
    // are models of BidirectionalIterator.

    info.windows = false;
    info.shading = false;
    info.lights = false;
    info.heatingSetpoint = false;

    for(bpt::ptree::value_type & v: pt.get_child("simulation")){
        if (v.first == "seed"){
            Utility::setSeed(v.second.get_value<int>());
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

std::vector<std::string> SimulationConfig::getZoneNameFromActivity(std::string activity){
    std::vector<std::string> zoneNames; //Default out of zone

    for (std::pair<std::string, zoneStruct> it : zones){
        std::vector<std::string> activities = splitZoneActivities(it.second.activity);
        for(std::string eachActivity: activities){
            if (eachActivity == activity)
            {
                zoneNames.push_back(it.first);
                break;
            }
        }
    }
    if(zoneNames.size() == 0){
      zoneNames.push_back("Out");
    }
    return zoneNames;
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


/* Deleted because not used
std::vector<std::string> SimulationConfig::getActivities(std::string* name){
    std::vector<std::string> activities;
    if (activeZone(name)){
        zoneStruct zone = SimulationConfig::zones.at(*name);
        activities = splitZoneActivities(zone.activity);
    }
    return activities;
}
*/

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

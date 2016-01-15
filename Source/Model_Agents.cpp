/*
 * File:   Comp_Agent.cpp
 * Author: jake
 *
 * Created on September 15, 2013, 4:21 PM
 */
#include <string>
#include <vector>
#include <list>
#include "SimulationConfig.h"
#include "Agent.h"
#include "State_Out.h"
#include "State_Present.h"
#include "State_Sleep.h"
#include "State_Passive.h"
#include "State_IT.h"
#include "State_Washing_Appliance.h"
#include "State_Audio_Visual.h"
#include "State_Cleaning.h"
#include "State_Cooking.h"
#include "State_Washing.h"
#include "State_Metabolic.h"
#include "Utility.h"
#include "Model_Presence.h"
#include "Model_Agents.h"

Model_Agents::Model_Agents(){
    stepCount = 0;
}

void Model_Agents::setup(){// create agents
    int populationSize = SimulationConfig::numberOfAgents();

    std::list<int> pop = Utility::randomIntList(populationSize, 0, populationSize);
    // setup each agent randomly
    for(int a: pop){
          population.push_back(Agent(a));
    }
    initialiseStates();
}

void Model_Agents::setZones(std::vector<Zone> zones){
    this->zones = zones;
}

void Model_Agents::initialiseStates(){
    State_Out out;
    matchStateToZone(out);
    stateMachine.addState(out);
    State_Present present;
    if (SimulationConfig::info.presencePage){
        State_IT it;
        matchStateToZone(it);
        it.getZonePtr()->getName();
        present.addState(it);
    }
    else{
        State_Sleep sleep;
        matchStateToZone(sleep);
        State_Passive passive;
        matchStateToZone(passive);
        State_Washing_Appliance washingAppliance;
        matchStateToZone(washingAppliance);
        State_Washing washing;
        matchStateToZone(washing);
        State_Audio_Visual audioVisual;
        matchStateToZone(audioVisual);
        State_Cleaning cleaning;
        matchStateToZone(cleaning);
        State_Cooking cooking;
        matchStateToZone(cooking);
        State_Metabolic metabolic;
        matchStateToZone(metabolic);
        State_IT it;
        matchStateToZone(it);
        present.addState(sleep);  //present.addState(sleep) <=> present.stateMachine.addState(sleep);
        present.addState(passive);
        present.addState(washingAppliance);
        present.addState(washing);
        present.addState(audioVisual);
        present.addState(cleaning);
        present.addState(cooking);
        present.addState(metabolic);
        present.addState(it);
    }
    stateMachine.addState(present);
}

void Model_Agents::matchStateToZone(State &s){
    std::string zoneName = SimulationConfig::getZoneNameFromActivity(s.getActivity());
    for(unsigned int i =0; i < zones.size(); i++){
        if(zoneName == zones[i].getName()){
            s.setZonePtr(&(zones[i]));
            break;
        }
    }
}


void Model_Agents::step(){
    std::list<int> pop = Utility::randomIntList(population.size(), 0, population.size());
    // step each agent randomly
    for(int a: pop){
        population[a].step(&stateMachine); // Every agents of the zone calculate their decisions
    }
    stepCount++;
    for(Zone &zone: zones){
        if(!zone.isActive()){
            continue;
        }
        setAgentCountForZone(&zone);
        if(SimulationConfig::info.windows){
            setAgentWindowDecisionForZone(&zone);
        }
        if(SimulationConfig::info.shading){
            setAgentBlindDecisionForZone(&zone);
        }
        if(SimulationConfig::info.lights){
            setAgentLightDecisionForZone(&zone);
        }
        if(SimulationConfig::info.heatingSetpoint){
            setAgentHeatingSetpointDecisionForZone(&zone);
        }
        setAgentGainsForZone(&zone);
        zone.step();
    }
}

void Model_Agents::setAgentGainsForZone(Zone *zone){
    double numberOfAgents = 0;
    double totalRadientGains = 0;
    double aveRadientGains = 0;

    for(Agent &agent: population){
        if (agent.isInZone(zone->getName())){
            numberOfAgents++;
            totalRadientGains += agent.getCurrentRadientGains(*zone);
            //std::cout << "zonename: " << zoneName << " totalgains " << totalRadientGains << std::endl;
        }
    }
    if (totalRadientGains > 0){
        aveRadientGains = totalRadientGains / numberOfAgents;
    }
    zone->setCurrentAgentGains(aveRadientGains);
}

void Model_Agents::setAgentWindowDecisionForZone(Zone *zone){
    double open = 0;
    double close = 0;
    double numberOfActiveAgents = 0;
    for(Agent &agent: population)
    {
        if (agent.InteractionOnZone(*zone))
        {
            numberOfActiveAgents++;
            double power = agent.getPower();
            if(agent.getDesiredWindowState(*zone))
            {
                open = open + power;
            }
            else
            {
                close = close + power;
            }
        }
    }
    if(numberOfActiveAgents > 0.0)
    {
        if(open < close)
        {
            zone->setWindowState(false);
        }
        else if (open > close)
        {
            zone->setWindowState(true);
        }
        else
        {
            zone->setWindowState(Utility::tossACoin());
        }
    }
}

void Model_Agents::setAgentBlindDecisionForZone(Zone *zone){
    double currentState = zone->getBlindState();
    double totalIncrease = 0;
    double totalDecrease = 0;
    double increase = 0;
    double decrease = 0;
    double same = 0;
    double increasePower = 0;
    double decreasePower = 0;
    double samePower = 0;

    for(Agent &agent: population){
        if (agent.InteractionOnZone(*zone)){
            double d = agent.getDesiredShadeState(*zone);
            double power = agent.getPower();

            if(d < currentState){
                decrease++;
                totalDecrease = totalDecrease + d;
                decreasePower = decreasePower + power;
            }
            else if(d > currentState){
                increase++;
                totalIncrease = totalIncrease + d;
                increasePower = increasePower + power;
            }
            else{
                same++;
                samePower = samePower + power;
            }
        }
    }
    totalDecrease = totalDecrease / decrease;
    totalIncrease = totalIncrease / increase;
    double state = currentState;
    if(samePower > increasePower && samePower > decreasePower){
        state = currentState;
    }
    else if (samePower < increasePower && increasePower > decreasePower){
        state = totalIncrease;
    }
    else if (samePower < decreasePower && increasePower < decreasePower){
        state = totalDecrease;
    }
    else if (samePower == increasePower && samePower > decreasePower){
        if (Utility::tossACoin()){
            state = totalIncrease;
        }
    }
    else if (samePower > increasePower && samePower == decreasePower){
        if (Utility::tossACoin()){
            state = totalDecrease;
        }
    }
    else if (samePower < increasePower && samePower < decreasePower && increasePower == decreasePower){
        if (Utility::tossACoin()){
            state = totalIncrease;
        }
        else{
            state = totalDecrease;
        }
    }
    else if ((samePower == increasePower) == decreasePower){
        double i = Utility::randomDouble(0,1);
        double d = Utility::randomDouble(0,1);
        double s = Utility::randomDouble(0,1);
        if(i > d && i > s){
            state = totalIncrease;
        }
        else if (d > s && d > i){
            state = totalDecrease;
        }
        else if (s > i && s > d){
            state = currentState;
        }
    }
    zone->setBlindState(state); // divide by the number of agents that want make the decision. To check
    //std::cout << increase << ", " << decrease << ", " << same << ", " << zone->getName() << ", " << zone->getBlindState() << std::endl;
}

void Model_Agents::setAgentLightDecisionForZone(Zone *zone){
    double open = 0;
    double close = 0;
    double numberOfActiveAgents = 0;
    for(Agent &agent: population){
        if (agent.InteractionOnZone(*zone)){
            numberOfActiveAgents++;
            double power = agent.getPower();
            if(agent.getDesiredLightState(*zone)){
                open = open + power;
            }
            else{
                close = close + power;
            }
        }
    }
    if(numberOfActiveAgents > 0.0){
        if(open < close){
            zone->setLightState(false);
        }
        else if (open > close){
            zone->setLightState(true);
        }
        else{
            zone->setLightState(Utility::tossACoin());
        }
    }
}

void Model_Agents::setAgentHeatingSetpointDecisionForZone(Zone *zone){
    double d = zone->getHeatingSetpointState();
    double TempPower = 0, totPower = 0;
    for(Agent &agent: population){
        if (agent.InteractionOnZone(*zone)){
            d = agent.getDesiredHeatingSetpointState(*zone);
            double power = agent.getPower();
            TempPower += d*power;
            totPower += power;
        }
    }
    if (totPower != 0){
        d = TempPower/totPower;
    }
    zone->setHeatingSetpointState(d);
}

void Model_Agents::setAgentCountForZone(Zone *zone){
    double numberOfAgents = 0;
    for(Agent &agent: population){
        if (agent.isInZone(zone->getName())){
            numberOfAgents++;
        }
    }
    double fractionsOfOccupants = numberOfAgents / (double)population.size();
    zone->setOccupantFraction(fractionsOfOccupants);
}


void Model_Agents::postprocess(){
    for(Agent &agent: population){
        agent.postprocess();
    }
}

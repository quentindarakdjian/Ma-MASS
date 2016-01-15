/*
 * File:   Comp_Agent.h
 * Author: jake
 *
 * Created on September 15, 2013, 4:21 PM
 */

#ifndef COMP_AGENT_H
#define	COMP_AGENT_H
#include <vector>
#include <memory>
#include "Agent.h"
#include "StateMachine.h"
#include "Zone.h"

class Model_Agents{
public:
    Model_Agents();

    void setup();
    void step();
    void setAgentGainsForZone(Zone *zone);
    void setAgentWindowDecisionForZone(Zone *zone);
    void setAgentLightDecisionForZone(Zone *zone);
    void setAgentHeatingSetpointDecisionForZone(Zone *zone);
    void setAgentBlindDecisionForZone(Zone *zone);
    void setAgentCountForZone(Zone *zone);
    void initialiseStates();
    void setZones(std::vector<Zone> zones);
    void postprocess();

private:
    void matchStateToZone(State &s);
    std::vector<Agent> population;
    int stepCount;
    StateMachine stateMachine; //StateMachine is included in Model_Agent
    std::vector<Zone> zones;
};

#endif	/* COMP_AGENT_H */

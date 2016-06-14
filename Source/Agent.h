/*
 * File:   Agent.h
 * Author: jake
 *
 * Created on September 15, 2013, 4:37 PM
 */

#ifndef AGENT_H
#define	AGENT_H
#include <unordered_map>
#include <string>
#include <memory>

#include "Model_Activity.h"
#include "Model_Presence.h"
#include "StateMachine.h"
#include "State.h"

#include "Model_Lights.h"
#include "Model_HeatingSetpoint.h"
#include "Model_ExternalShading.h"

#include "Agent_Action_Window.h"
#include "Agent_Action_Lights.h"
#include "Agent_Action_Heating_Setpoint.h"
#include "Agent_Action_Shades.h"
#include "Agent_Action_Heat_Gains.h"
/**
 * @brief The Agent
 * @details Contains all information about the occupants and there associated interactions
 */

struct ActionValues {
    bool lightState;
    double heatingSetpointState;
    bool windowState;
    double shadeState;
    double ppd;
    double pmv;
    double heatgains;
};

class Agent {
public:
    Agent();
    Agent(int newId);


    void step(StateMachine *sm);
    void zoneInteractions();
    void postprocess();
    void model_presenceFromActivities();
    void model_presenceFromPage();
    bool presentAt(int step) const;
    bool isInZone(std::string zoneName) const;
    bool InteractionOnZone(const Zone &zone) const;
    int presentForFutureSteps() const;
    double getCurrentRadientGains(const Zone &zone) const;
    double getPower() const;
    int getAge() const;
    //int getIncome() const;


    void interactWithZone(const Zone &zone);
    bool getDesiredLightState(const Zone &zone) const;
    double getDesiredHeatingSetpointState(const Zone &zone) const;
    bool getDesiredWindowState(const Zone &zone) const;
    bool getDesiredShadeState(const Zone &zone) const;
    std::string getLocationType(int step,StateMachine *sm);


private:
    int id;
    int action;
    int pmv;
    int previous_pmv;
    double power;
    int age;
    double currentRadientGains;
    double metabolicRate; /** Metabolic Rate of the occupant */
    double clo; /** clothing value of the occupant */

    std::vector<double> activities;
    //std::vector<int> presence;
    std::vector<int> presenceForFutureSteps;
    std::vector<int> currentDurationOfPresenceState;

    std::string jobType; /** What type of schedule has the agent - For office building simulations */
    std::string bedroom; /** Which bedroom the occupant sleeps in */
    std::string office; /** WhichoOffice the occupant works in */
    std::string activity; /** The activities performed in the location */
    Model_Presence presence;
    State state; /** Occupants current state */
    State previousState; /** Occupants previous state */

    Agent_Action_Window aaw;
    Agent_Action_Lights aal;
    Agent_Action_Heating_Setpoint aahs;
    Agent_Action_Shades aas;
    Agent_Action_Heat_Gains aahg;

    std::vector<int> availableActions;

    std::unordered_map<std::string, ActionValues> zoneToInteraction;
    void model_pastAndFutureDurations();
    void model_activity();

    void actionStep(int action, ActionValues *interaction, const Zone &zone, bool inZone, bool preZone);


    //void rLearn(const Zone &zone, ActionValues *interaction);

    Zone* updateLocation(const State& state) const;
    bool currentlyInZone(const Zone &zone) const;
    bool previouslyInZone(const Zone &zone) const;
    bool calculateLightInteractionsOnZone(const Zone &zone);
    bool calculateWindowInteractionsOnZone(const Zone &zone);
    double calculateHeatingSetpointInteractionInteractionsOnZone(const Zone &zone);
    double calculateExternalShadeInteractionsOnZone(const Zone &zone);
    double calculateMetabolicHeatGainsOnZone(const Zone &zone);
    double getPreviousDurationOfAbsenceState() const;
    double getCurrentDurationOfPresenceState() const;
    double getFutureDurationOfPresenceState() const;
};

#endif	/* AGENT_H */

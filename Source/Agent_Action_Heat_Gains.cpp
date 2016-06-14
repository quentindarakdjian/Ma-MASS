
#include <iostream>
#include "DataStore.h"
#include "Model_HeatGains.h"
#include "Agent_Action_Heat_Gains.h"

Agent_Action_Heat_Gains::Agent_Action_Heat_Gains(){
    name = "HeatGains";
}

void Agent_Action_Heat_Gains::setup(int agentid){
        this->id = agentid;

        idAsString = std::to_string(id);
        DataStore::addVariable("AgentGains" + idAsString);
        DataStore::addVariable("Agent_Metabolic_Rate_" + idAsString);
        DataStore::addVariable("Agent_clo_" + idAsString);
        DataStore::addVariable("Agent_ppd_" + idAsString);
        DataStore::addVariable("Agent_pmv_" + idAsString);
}

void Agent_Action_Heat_Gains::prestep(double clo, double metabolicRate){
    this->clo = clo;
    this->metabolicRate = metabolicRate;
}

void Agent_Action_Heat_Gains::step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities){
        ppd = 5;
        pmv = 0;
        result = 0;
        if (inZone) {
                Model_HeatGains h;
                /**
                 * Calculates the Fanger pmv and sets the instance varibles related to results.
                 * @param metabolicRate Metabolic Rate
                 * @param partialWaterPressure partial water vapour kPa
                 * @param meanRadiantTemperature mean radiant temperature C
                 * @param externalWork external work
                 * @param ta air temperature
                 * @param clo Clothing value
                 * @param airVelocity Air velocity
                 */

                double airTemp = zone.getMeanAirTemperature();
                double airHumid = zone.getAirRelativeHumidity();
                double meanRadient = zone.getMeanRadiantTemperature();

                /*
                std::cout << "metabolicRate: " << metabolicRate << std::endl;
                std::cout << "airHumid: " << airHumid << std::endl;
                std::cout << "meanRadient: " << meanRadient << std::endl;
                std::cout << "airTemp: " << airTemp << std::endl;
                std::cout << "clo: " << clo << std::endl;
                */

                h.calculate(metabolicRate, airHumid, meanRadient, 0, airTemp, clo, 0);
                result = h.getAllHeatGains();
                ppd = h.getPpd();
                pmv = h.getPmv();
        }
        std::string name = "AgentGains" + idAsString;
        DataStore::addValue(name.c_str(), result);
        name = "Agent_Metabolic_Rate_" + idAsString;
        DataStore::addValue(name.c_str(), metabolicRate);
        name = "Agent_clo_" + idAsString;
        DataStore::addValue(name.c_str(), clo);
        name = "Agent_ppd_" + idAsString;
        DataStore::addValue(name.c_str(), ppd);
        name = "Agent_pmv_" + idAsString;
        DataStore::addValue(name.c_str(), pmv);
}

double Agent_Action_Heat_Gains::getPMV(){
    return pmv;
}

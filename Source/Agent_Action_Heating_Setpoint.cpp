

#include <iostream>
#include "Model_HeatingSetpoint.h"
#include "Agent_Action_Heating_Setpoint.h"
#include "SimulationConfig.h"
#include "Utility.h"
#include "DataStore.h"
#include "Agent.h"

Agent_Action_Heating_Setpoint::Agent_Action_Heating_Setpoint(){
    name = "Heating_Setpoint";
}

void Agent_Action_Heating_Setpoint::setup(int age){
    //----------------------------int populationSize = SimulationConfig::numberOfAgents();
    this->age = age;
    double alpha = Utility::randomDoubleNormal(14.224, 0.303);
    double coeffLocalisation = Model_HeatingSetpoint::coeffLocalisation(SimulationConfig::building.localisation);
    double coeffRoomThermostat = Model_HeatingSetpoint::coeffRoomThermostat(SimulationConfig::building.roomThermostat);
    double coeffThermostaticRadiatorValve = Model_HeatingSetpoint::coeffThermostaticRadiatorValve(SimulationConfig::building.thermostaticRadiatorValve);
    double coeffCentralHeatingHoursReported = Model_HeatingSetpoint::coeffCentralHeatingHoursReported(Utility::randomDoubleNormal(9.84, 5.3)); // Determined randomly
    double coeffRegularHeatingPattern = Model_HeatingSetpoint::coeffRegularHeatingPattern(Utility::randomBool(0.88)); // Determined randomly
    double coeffHouseHoldSize = Model_HeatingSetpoint::coeffHouseHoldSize(SimulationConfig::numberOfAgents());
    double coeffHouseHoldIncome = Model_HeatingSetpoint::coeffHouseHoldIncome(SimulationConfig::building.houseHoldIncome);
    double coeffAge = Model_HeatingSetpoint::coeffAge(age);
    double coeffTenureType = Model_HeatingSetpoint::coeffTenureType(SimulationConfig::building.tenureType);
    double coeffTypology = Model_HeatingSetpoint::coeffTypology(SimulationConfig::building.typology);
    double coeffMainFuel = Model_HeatingSetpoint::coeffMainFuel(SimulationConfig::building.mainFuel);
    double coeffAdditionalFuel = Model_HeatingSetpoint::coeffAdditionalFuel(SimulationConfig::building.additionalFuel);
    double coeffYearOfConstruction = Model_HeatingSetpoint::coeffYearOfConstruction(SimulationConfig::building.yearOfConstruction);
    double coeffRoofInsulationThickness = Model_HeatingSetpoint::coeffRoofInsulationThickness(SimulationConfig::building.roofInsulationThickness);
    double coeffExtendOfDoubleGlazing = Model_HeatingSetpoint::coeffExtendOfDoubleGlazing(SimulationConfig::building.extendOfDoubleGlazing);
    double coeffWallUValue = Model_HeatingSetpoint::coeffWallUValue(SimulationConfig::building.wallUValue);

    temperatureSetpointBase = alpha + coeffLocalisation + coeffRoomThermostat + coeffThermostaticRadiatorValve +
                                coeffCentralHeatingHoursReported + coeffRegularHeatingPattern + coeffHouseHoldSize +
                                coeffHouseHoldIncome + coeffAge + coeffTenureType + coeffTypology + coeffMainFuel +
                                coeffAdditionalFuel + coeffYearOfConstruction + coeffRoofInsulationThickness +
                                coeffExtendOfDoubleGlazing + coeffWallUValue;
    std::cout<<"The base temperature of the agent is: "<<temperatureSetpointBase<<" *C."<<std::endl;
    passivity = Utility::randomInt(1,3);
    std::cout<<"Passivity of the agent is: "<<passivity<<std::endl;
}

void Agent_Action_Heating_Setpoint::step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities){
//    result = temperatureSetpointBase;
    heatingSetpointState = zone.getHeatingSetpointState();
    double indoorRelativeHumidity = zone.getAirRelativeHumidity();
    double indoorTemperature = zone.getMeanAirTemperature();
    // dailyTemp
    double outdoorTemperature = DataStore::getValue("EnvironmentSiteOutdoorAirDrybulbTemperature");
    outDoorTemperatures.push_back(outdoorTemperature);
    if (outDoorTemperatures.size() > SimulationConfig::info.timeStepsPerHour*24){
        outDoorTemperatures.pop_front();
    }
    double dailyTemperature = 0;
    for (double temp : outDoorTemperatures){
        dailyTemperature += temp;
    }
    dailyTemperature = dailyTemperature / (double) outDoorTemperatures.size();

    Model_HeatingSetpoint m_heatingSetpointUsage;


    double randUp = Utility::randomDouble(0,1);
    double randDown = Utility::randomDouble(0,1);
    double pUp = m_heatingSetpointUsage.pUp(passivity, indoorTemperature);
    double pDown = m_heatingSetpointUsage.pDown(passivity, indoorTemperature);
    if (randUp < pUp){
        double heatingSetpointStateTest = m_heatingSetpointUsage.inZone(temperatureSetpointBase, dailyTemperature);
        //if (heatingSetpointStateTest > heatingSetpointState){
            heatingSetpointState = heatingSetpointStateTest;
        //} else {}
    }
    // Down
    if (randDown < pDown){
        double heatingSetpointStateTest = m_heatingSetpointUsage.inZone(temperatureSetpointBase, dailyTemperature);
        //if (heatingSetpointStateTest < heatingSetpointState){
            heatingSetpointState = heatingSetpointStateTest;
        //} else {}
    }
    // Nothing
    else{
        heatingSetpointState = zone.getHeatingSetpointState();
    }

// Social info //
    if (SimulationConfig::info.social){
        int stepCount = SimulationConfig::getStepCount();
        if (activities.at(stepCount) == 0 || activities.at(stepCount == 9)){ // If agent sleep or is out then temperature of the zone is decreased
            heatingSetpointState = heatingSetpointState - Utility::randomDouble(-0.5, 0);
        }
    }
    result = heatingSetpointState;
}


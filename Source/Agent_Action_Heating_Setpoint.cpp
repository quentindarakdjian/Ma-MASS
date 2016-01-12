#include <iostream>
#include "Model_HeatingSetpoint.h"
#include "Agent_Action_Heating_Setpoint.h"
#include "SimulationConfig.h"
#include "Utility.h"
#include "DataStore.h"

Agent_Action_Heating_Setpoint::Agent_Action_Heating_Setpoint(){
    name = "Heating_Setpoint";
}

void Agent_Action_Heating_Setpoint::setup(int age){
    //----------------------------int populationSize = SimulationConfig::numberOfAgents();
    this->age = age;
    double alpha = Utility::randomDouble(13.618, 14.830);
    double coeffLocalisation = Model_HeatingSetpoint::coeffLocalisation(SimulationConfig::building.localisation);
    double coeffRoomThermostat = Model_HeatingSetpoint::coeffRoomThermostat(SimulationConfig::building.roomThermostat);
    double coeffThermostatSetting = Model_HeatingSetpoint::coeffThermostatSetting(SimulationConfig::building.thermostatSetting);
    double coeffThermostaticRadiatorValve = Model_HeatingSetpoint::coeffThermostaticRadiatorValve(SimulationConfig::building.thermostaticRadiatorValve);
    double coeffCentralHeatingHoursReported = Model_HeatingSetpoint::coeffCentralHeatingHoursReported(SimulationConfig::building.centralHeatingHoursReported);
    double coeffRegularHeatingPattern = Model_HeatingSetpoint::coeffRegularHeatingPattern(SimulationConfig::building.regularHeatingPattern);
    double coeffAutomaticTimer = Model_HeatingSetpoint::coeffAutomaticTimer(SimulationConfig::building.automaticTimer);
   double coeffHouseHoldSize = Model_HeatingSetpoint::coeffHouseHoldSize(SimulationConfig::numberOfAgents());
    double coeffHouseHoldIncome = Model_HeatingSetpoint::coeffHouseHoldIncome(SimulationConfig::building.houseHoldIncome);
   double coeffAge = Model_HeatingSetpoint::coeffAge(age);
    double coeffTenureType = Model_HeatingSetpoint::coeffTenureType(SimulationConfig::building.tenureType);
    double coeffTypology = Model_HeatingSetpoint::coeffTypology(SimulationConfig::building.typology);
    double coeffGasCentralHeating = Model_HeatingSetpoint::coeffGasCentralHeating(SimulationConfig::building.gasCentralHeating);
    double coeffNonCentralHeating = Model_HeatingSetpoint::coeffNonCentralHeating(SimulationConfig::building.nonCentralHeating);
    double coeffElectricityIsMainFuel = Model_HeatingSetpoint::coeffElectricityIsMainFuel(SimulationConfig::building.electricityIsMainFuel);
    double coeffAdditionalGasHeatingInLivingRoom = Model_HeatingSetpoint::coeffAdditionalGasHeatingInLivingRoom(SimulationConfig::building.additionalGasHeatingInLivingRoom);
    double coeffAdditionalElectricityHeatingInLivingRoom = Model_HeatingSetpoint::coeffAdditionalElectricityHeatingInLivingRoom(SimulationConfig::building.additionalElectricityHeatingInLivingRoom);
    double coeffAdditionalOtherHeatingInLivingRoom = Model_HeatingSetpoint::coeffAdditionalOtherHeatingInLivingRoom(SimulationConfig::building.additionalOtherHeatingInLivingRoom);
    double coeffYearOfConstruction = Model_HeatingSetpoint::coeffYearOfConstruction(SimulationConfig::building.yearOfConstruction);
    double coeffRoofInsulationThickness = Model_HeatingSetpoint::coeffRoofInsulationThickness(SimulationConfig::building.roofInsulationThickness);
    double coeffExtendOfDoubleGlazing = Model_HeatingSetpoint::coeffExtendOfDoubleGlazing(SimulationConfig::building.extendOfDoubleGlazing);
    double coeffWallUValue = Model_HeatingSetpoint::coeffWallUValue(SimulationConfig::building.wallUValue);

    temperatureSetpointBase = alpha + coeffLocalisation + coeffRoomThermostat + coeffLocalisation +
                                coeffCentralHeatingHoursReported + coeffCentralHeatingHoursReported +
                                coeffRegularHeatingPattern + coeffAutomaticTimer + coeffHouseHoldSize + coeffHouseHoldIncome +
                                coeffAge + coeffTenureType + coeffTypology + coeffGasCentralHeating +
                                coeffNonCentralHeating + coeffElectricityIsMainFuel + coeffAdditionalGasHeatingInLivingRoom +
                                coeffAdditionalElectricityHeatingInLivingRoom + coeffAdditionalOtherHeatingInLivingRoom +
                                coeffYearOfConstruction + coeffRoofInsulationThickness + coeffExtendOfDoubleGlazing +
                                coeffWallUValue;
    std::cout<<"The base temperature of Agent is: "<<temperatureSetpointBase<<" *C."<<std::endl;
}

void Agent_Action_Heating_Setpoint::step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities){
    result = 0;
    //double heatingSetpointState = zone.getHeatingSetpointState();

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

    double coeffOutdoorTemperature = Utility::randomDouble(0.006, 0.098);
    double coeffOutdoorTemperature2 = Utility::randomDouble(0.009, 0.016);
    Model_HeatingSetpoint m_heatingSetpointUsage;
    double heatingSetpointState = m_heatingSetpointUsage.inZone(temperatureSetpointBase, dailyTemperature, coeffOutdoorTemperature, coeffOutdoorTemperature2);
    int stepCount = SimulationConfig::getStepCount();
    if (activities.at(stepCount) == 0 || activities.at(stepCount == 9)){ // If agent sleep or is out then temperature of the zone is decreased
          heatingSetpointState = heatingSetpointState - Utility::randomDouble(-0.5, 0);;
    }
    result = heatingSetpointState;
}


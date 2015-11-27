#include <iostream>
#include "Model_HeatingSetpoint.h"
#include "Agent_Action_Heating_Setpoint.h"
#include "Building.h"
#include "SimulationConfig.h"
#include "DataStore.h"
#include "Utility.h"

Agent_Action_Heating_Setpoint::Agent_Action_Heating_Setpoint(){
    name = "Heating_Setpoint";
}

void Agent_Action_Heating_Setpoint::setup(){
    //----------------------------int populationSize = SimulationConfig::numberOfAgents();
    double alpha = Utility::randomDouble(13.618, 14.830);

    double coeffLocalisation = Model_HeatingSetpoint::coeffLocalisation(SimulationConfig::building.localisation);
    double coeffRoomThermostat = Model_HeatingSetpoint::coeffRoomThermostat(SimulationConfig::building.roomThermostat);
    double coeffThermostatSetting = Model_HeatingSetpoint::coeffThermostatSetting(SimulationConfig::building.thermostatSetting);
    double coeffThermostaticRadiatorValve = Model_HeatingSetpoint::coeffThermostaticRadiatorValve(SimulationConfig::building.thermostaticRadiatorValve);
    double coeffCentralHeatingHoursReported = Model_HeatingSetpoint::coeffCentralHeatingHoursReported(SimulationConfig::building.centralHeatingHoursReported);
    double coeffRegularHeatingPattern = Model_HeatingSetpoint::coeffRegularHeatingPattern(SimulationConfig::building.regularHeatingPattern);
    double coeffAutomaticTimer = Model_HeatingSetpoint::coeffAutomaticTimer(SimulationConfig::building.automaticTimer);

    //-----------------------------double coeffHouseHoldSize = Model_HeatingSetpoint::coeffHouseHoldSize(populationSize);
    double coeffHouseHoldIncome = Model_HeatingSetpoint::coeffHouseHoldIncome(SimulationConfig::building.houseHoldIncome);
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
                                coeffRegularHeatingPattern + coeffAutomaticTimer + coeffHouseHoldIncome +
                                coeffTenureType + coeffTypology + coeffGasCentralHeating +
                                coeffNonCentralHeating + coeffElectricityIsMainFuel + coeffAdditionalGasHeatingInLivingRoom +
                                coeffAdditionalElectricityHeatingInLivingRoom + coeffAdditionalOtherHeatingInLivingRoom;
}

void Agent_Action_Heating_Setpoint::step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities){
    double heatingSetpointState = zone.getHeatingSetpointState();
    double outdoorTemperature = DataStore::getValue("EnvironmentSiteOutdoorAirDrybulbTemperature");


    heatingSetpointState = temperatureSetpointBase + 0.052*outdoorTemperature + 0.012*pow(outdoorTemperature,2);
    if (heatingSetpointState>24.5){
        heatingSetpointState=24.5;
    }
    result = heatingSetpointState;
}




/*
void Agent_Action_Heating_Setpoint::step(const Zone& zone, bool inZone, bool previouslyInZone, const std::vector<double> &activities){
    double Tempint = zone.getMeanAirTemperature();
    double heatingSetpointState = zone.getHeatingSetpointState();
    Model_HeatingSetpoint m_heatingSetpointUsage; //At each time step the object m_heatingSetpointUsage is created??
    //Setpoint reset twice a day
    int intLenghtOfTimestep = SimulationConfig::lengthOfTimestep(); // lenghtOfTimestep is a double
    if(SimulationConfig::getStepCount() % (1440 / 2 / intLenghtOfTimestep) == 0){
        if (inZone && !previouslyInZone) {
            heatingSetpointState = m_heatingSetpointUsage.arrival(heatingSetpointState, Tempint, gender);
        }
        else if (inZone && previouslyInZone ) {
            heatingSetpointState = m_heatingSetpointUsage.intermediate(heatingSetpointState, Tempint, gender);
        }
        else if (!inZone && previouslyInZone ) {
            heatingSetpointState = m_heatingSetpointUsage.departure(heatingSetpointState, Tempint);
        }
    }
    if (heatingSetpointState < 15){
        heatingSetpointState = 15;
    }
    if (heatingSetpointState > 25){
        heatingSetpointState = 25;
    }
    result = heatingSetpointState;
}
*/

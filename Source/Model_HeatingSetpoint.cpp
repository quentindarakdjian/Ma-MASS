#include <algorithm>
#include "Utility.h"
#include "Model_HeatingSetpoint.h"

Model_HeatingSetpoint::Model_HeatingSetpoint() {
}
/*
void Model_HeatingSetpoint::buildingTemperatureBase (double alpha, std::string localisation, bool T_stat, double thermostatSetting){
    double temperatureBase = alpha + coeffLocalisation(localisation) + coeffT_stat(T_stat) + coeffThermostatSetting(thermostatSetting);
    std::cout<<"temperatureBase in Building.cpp is: "<<temperatureBase<<std::endl;
    return temperatureBase;
}
*/

double Model_HeatingSetpoint::coeffLocalisation(std::string localisation){
    double coeffLocalisation = 0;
    if (localisation == "Urban"){
        coeffLocalisation = 0;
    }
    else {
        coeffLocalisation = Utility::randomDouble(0, -1);
    }
    return coeffLocalisation;
}

double Model_HeatingSetpoint::coeffRoomThermostat(bool roomThermostat){
    double coeffRoomThermostat = 0;
    if (roomThermostat){
        coeffRoomThermostat = Utility::randomDouble(-0.289, -0.183);
    }
    return coeffRoomThermostat;
}

double Model_HeatingSetpoint::coeffThermostatSetting(double thermostatSetting){
    int cat = 0;
    if (thermostatSetting > 18 && thermostatSetting <= 20){
        cat = 1;
    }
    else if (thermostatSetting > 20 && thermostatSetting <= 22){
        cat = 2;
    }
    else {
        cat = 3;
    }
    double random = Utility::randomDouble(0.001, 0.069);
    double coeffThermostatSetting = cat*random;
    return coeffThermostatSetting;
}

double Model_HeatingSetpoint::coeffThermostaticRadiatorValve(bool thermostaticRadiatorValve){
    double coeffThermostaticRadiatorValve = 0;
    if (thermostaticRadiatorValve){
        coeffThermostaticRadiatorValve = Utility::randomDouble(-0.244, -0.093);
    }
    return coeffThermostaticRadiatorValve;
}

double Model_HeatingSetpoint::coeffCentralHeatingHoursReported(double centralHeatingHoursReported){
    double random = Utility::randomDouble(0.058, 0.081);
    double coeffCentralHeatingHoursReported = centralHeatingHoursReported*random;
    return coeffCentralHeatingHoursReported;
}

double Model_HeatingSetpoint::coeffRegularHeatingPattern(bool regularHeatingPattern){
    double coeffRegularHeatingPattern = 0;
    if (regularHeatingPattern){
        coeffRegularHeatingPattern = Utility::randomDouble(0.978, 1.399);
    }
    return coeffRegularHeatingPattern;
}

double Model_HeatingSetpoint::coeffAutomaticTimer(bool automaticTimer){
    double coeffAutomaticTimer = 0;
    if (automaticTimer){
        coeffAutomaticTimer = Utility::randomDouble(-0.08, 0.018);
    }
    return coeffAutomaticTimer;
}

/*-------------------------------
double Model_HeatingSetpoint::coeffHouseHoldSize = Model_HeatingSetpoint::coeffHouseHoldSize(int populationSize){
    double random = Utility::randomDouble(0.196, 0.304);
    double coeffHouseHoldSize = SimulationConfig::numberOfAgents()*random;
    return coeffHouseHoldSize;
}
-------------------------------------------*/

double Model_HeatingSetpoint::coeffHouseHoldIncome(double houseHoldIncome){
    // The initial model was calibrated on £.
    // The conversion to € is approximative and based on the change rate of November 2015
    // £1 = 1.4€
    int cat = 0;
    if (houseHoldIncome > 7 && houseHoldIncome <= 15){
        cat = 1;
    }
    else if (houseHoldIncome > 15 && houseHoldIncome <= 30){
        cat = 2;
    }
    else if (houseHoldIncome > 30 && houseHoldIncome <= 50){
        cat = 3;
    }
    else if (houseHoldIncome > 50 && houseHoldIncome <= 73){
        cat = 4;
    }
    else if (houseHoldIncome > 73 && houseHoldIncome <= 133){
        cat = 5;
    }
    else {
        cat = 6;
    }
    double random = Utility::randomDouble(0.043, 0.124);
    double coeffHouseHoldIncome = cat*random;
    return coeffHouseHoldIncome;
}

double Model_HeatingSetpoint::coeffTenureType(std::string tenureType){
    double coeffTenureType = 0;
    // OwnerOccupier, PrivatelyRented, CouncilTenant, HousingAssociation
    if (tenureType == "PrivatelyRented"){
        coeffTenureType = Utility::randomDouble(0.814, 1.066);;
    }
    else if (tenureType == "CouncilTenant"){
        coeffTenureType = Utility::randomDouble(1.222, 1.525);
    }
    else if (tenureType == "HousingAssociation"){
        coeffTenureType = Utility::randomDouble(0.341, 0.555);
    }
    return coeffTenureType;
}

double Model_HeatingSetpoint::coeffTypology(std::string typology){
    double coeffTypology = 0;
    // DetachedHouse, Semi-Detached , TerracedHouse, NotAHouse
    if (typology == "Semi-Detached"){
        coeffTypology = Utility::randomDouble(0.591, 0.796);;
    }
    else if (typology == "TerracedHouse"){
        coeffTypology = Utility::randomDouble(0.538, 0.676);
    }
    else if (typology == "NotAHouse"){
        coeffTypology = Utility::randomDouble(0.452, 0.630);
    }
    return coeffTypology;
}

double Model_HeatingSetpoint::coeffGasCentralHeating(bool gasCentralHeating){
    double coeffGasCentralHeating = 0;
    if (gasCentralHeating){
        coeffGasCentralHeating = Utility::randomDouble(-0.657, -0.470);
    }
    return coeffGasCentralHeating;
}

double Model_HeatingSetpoint::coeffNonCentralHeating(bool nonCentralHeating){
    double coeffNonCentralHeating = 0;
    if (nonCentralHeating){
        coeffNonCentralHeating = Utility::randomDouble(0.009,0.108);
    }
    return coeffNonCentralHeating;
}

double Model_HeatingSetpoint::coeffElectricityIsMainFuel(bool electricityIsMainFuel){
    double coeffElectricityIsMainFuel = 0;
    if (electricityIsMainFuel){
        coeffElectricityIsMainFuel = Utility::randomDouble(0.700, 1.315);
    }
    return coeffElectricityIsMainFuel;
}

double Model_HeatingSetpoint::coeffAdditionalGasHeatingInLivingRoom(bool additionalGasHeatingInLivingRoom){
    double coeffAdditionalGasHeatingInLivingRoom = 0;
    if (additionalGasHeatingInLivingRoom){
        coeffAdditionalGasHeatingInLivingRoom = Utility::randomDouble(-0.135, -0.006);
    }
    return coeffAdditionalGasHeatingInLivingRoom;
}

double Model_HeatingSetpoint::coeffAdditionalElectricityHeatingInLivingRoom(bool additionalElectricityHeatingInLivingRoom){
    double coeffAdditionalElectricityHeatingInLivingRoom = 0;
    if (additionalElectricityHeatingInLivingRoom){
        coeffAdditionalElectricityHeatingInLivingRoom = Utility::randomDouble(-0.284, -0.106);
    }
    return coeffAdditionalElectricityHeatingInLivingRoom;
}

double Model_HeatingSetpoint::coeffAdditionalOtherHeatingInLivingRoom(bool additionalOtherHeatingInLivingRoom){
    double coeffAdditionalOtherHeatingInLivingRoom = 0;
    if (additionalOtherHeatingInLivingRoom){
        coeffAdditionalOtherHeatingInLivingRoom = Utility::randomDouble(-1.129, -0.902);
    }
    return coeffAdditionalOtherHeatingInLivingRoom;
}


double Model_HeatingSetpoint::coeffYearOfConstruction(double yearOfConstruction){
    int cat = 0;
    if (yearOfConstruction <= 1850 ){
        cat = 0;
    }
    if (yearOfConstruction > 1850 && yearOfConstruction <= 1900){
        cat = 1;
    }
    else if (yearOfConstruction > 1900 && yearOfConstruction <= 1919){
        cat = 2;
    }
    else if (yearOfConstruction > 1919 && yearOfConstruction <= 1945){
        cat = 3;
    }
    else if (yearOfConstruction > 1945 && yearOfConstruction <= 1965){
        cat = 4;
    }
    else if (yearOfConstruction > 1965 && yearOfConstruction <= 1975){
        cat = 5;
    }
    else if (yearOfConstruction > 1975 && yearOfConstruction <= 1981){
        cat = 6;
    }
    else if (yearOfConstruction > 1981 && yearOfConstruction <= 1991){
        cat = 7;
    }
    else if (yearOfConstruction > 1991 && yearOfConstruction <= 2001){
        cat = 8;
    }
    else if (yearOfConstruction > 2001){
        cat = 9;
    }
    double random = Utility::randomDouble(0.022, 0.062);
    double coeffYearOfConstruction = cat*random;
    return coeffYearOfConstruction;
}

double Model_HeatingSetpoint::coeffRoofInsulationThickness(double roofInsulationThickness){
    int cat = 0;
    if (roofInsulationThickness == 0 ){
        cat = 0;
    }
    if (roofInsulationThickness > 0 && roofInsulationThickness <= 25){
        cat = 1;
    }
    else if (roofInsulationThickness > 25 && roofInsulationThickness <= 50){
        cat = 2;
    }
    else if (roofInsulationThickness > 50 && roofInsulationThickness <= 75){
        cat = 3;
    }
    else if (roofInsulationThickness > 75 && roofInsulationThickness <= 100){
        cat = 4;
    }
    else if (roofInsulationThickness > 100 && roofInsulationThickness <= 150){
        cat = 5;
    }
    else if (roofInsulationThickness > 150 && roofInsulationThickness <= 200){
        cat = 6;
    }
    else if (roofInsulationThickness > 200){
        cat = 7;
    }
    double random = Utility::randomDouble(0.109, 0.142);
    double coeffRoofInsulationThickness = cat*random;
    return coeffRoofInsulationThickness;
}

double Model_HeatingSetpoint::coeffExtendOfDoubleGlazing(std::string extendOfDoubleGlazing){
    int cat = 0;
    if (extendOfDoubleGlazing == "None" ){
        cat = 0;
    }
    if (extendOfDoubleGlazing == "LessThanHalf"){
        cat = 1;
    }
    else if (extendOfDoubleGlazing == "AboutHalf"){
        cat = 2;
    }
    else if (extendOfDoubleGlazing == "MoreThanHalf"){
        cat = 3;
    }
    else if (extendOfDoubleGlazing == "AllWindows"){
        cat = 4;
    }
    double random = Utility::randomDouble(0.158, 0.217);
    double coeffExtendOfDoubleGlazing = cat*random;
    return coeffExtendOfDoubleGlazing;
}

double Model_HeatingSetpoint::coeffWallUValue(double wallUValue){
    int cat = 0;
    if (wallUValue <= 0.4 ){
        cat = 0;
    }
    if (wallUValue > 0.4 && wallUValue <= 0.6){
        cat = 1;
    }
    else if (wallUValue > 0.6 && wallUValue <= 1.6){
        cat = 2;
    }
    else if (wallUValue > 1.6){
        cat = 3;
    }
    double random = Utility::randomDouble(0.043, 0.108);
    double coeffWallUValue = cat*random;
    return coeffWallUValue;
}



/*
double Model_HeatingSetpoint::arrival(double state, double Tempint, bool gender){
    double HSPMin = 19; //HSP for HeatingSetpoint
    double HSPMax = 25;
    double HSPdesired = Utility::randomDouble(HSPMin, HSPMax);
    double currentHeatingSetpointState = state;
    if (Tempint + 1 < HSPdesired) {  //Use of the PMV instead of that
        currentHeatingSetpointState = currentHeatingSetpointState + Utility::randomDouble(2, 3);
    }
    if (gender) {  //If woman temperature increased between 0 and 0.5 °C)
        currentHeatingSetpointState = currentHeatingSetpointState + Utility::randomDouble(0, 0.5);
    }
    else { //If man temperature decrease between 0 and 0.5 °C)
        currentHeatingSetpointState = currentHeatingSetpointState - Utility::randomDouble(0, 0.5);
    }
    return currentHeatingSetpointState;
}

double Model_HeatingSetpoint::intermediate(double state, double Tempint, bool gender){
    double HSPMin = 19;
    double HSPMax = 25;
    double HSPdesired = Utility::randomDouble(HSPMin, HSPMax);
    double currentHeatingSetpointState = state;
    if (Tempint + 2 < HSPdesired) {  //Use of the PMV instead of that
        currentHeatingSetpointState = currentHeatingSetpointState + Utility::randomDouble(2, 3);
    }
    if (gender) {  //If woman, HeatingSetpoint increased between 0 and 0.5 °C)
        currentHeatingSetpointState = currentHeatingSetpointState + Utility::randomDouble(0, 0.5);
    }
    else { //If man HeatingSetpoint decrease between 0 and 0.5 °C)
        currentHeatingSetpointState = currentHeatingSetpointState - Utility::randomDouble(0, 0.5);
    }
    return currentHeatingSetpointState;
}

double Model_HeatingSetpoint::departure(double state, double Tempint){
    double currentHeatingSetpointState = state;
    if (Tempint > 20){
        currentHeatingSetpointState = currentHeatingSetpointState - Utility::randomDouble(1, 2);
    }
    return currentHeatingSetpointState;
}
*/

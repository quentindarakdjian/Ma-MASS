// Copyright AI Environnement 2017

#include <algorithm>
#include "DataStore.h"
#include "Utility.h"
#include "Model_HeatingSetpoint.h"

Model_HeatingSetpoint::Model_HeatingSetpoint() {
}

//-------Static-------//

double Model_HeatingSetpoint::coeffLocalisation(std::string localisation){
    double coeffLocalisation = 0;
    double prob = Utility::randomDouble(0,1);
    if (localisation == "Unknown"){
        if (prob < 0.775){ // http://www.insee.fr/fr/ffc/ipweb/ip1364/ip1364.pdf
            localisation = "Urban";
        }
        else{
            localisation = "Rural";
        }
    }
    if (localisation == "Urban"){
        coeffLocalisation = 0;
    }
    else if (localisation == "Rural"){
        coeffLocalisation = Utility::randomDoubleNormal(-1.620, -0.309);
    }
    return coeffLocalisation;
}

double Model_HeatingSetpoint::coeffRoomThermostat(bool roomThermostat){
    double coeffRoomThermostat = 0;
    if (roomThermostat){
        coeffRoomThermostat = Utility::randomDoubleNormal(-0.236, 0.027);
    }
    return coeffRoomThermostat;
}

double Model_HeatingSetpoint::coeffThermostaticRadiatorValve(bool thermostaticRadiatorValve){
    double coeffThermostaticRadiatorValve = 0;
    if (thermostaticRadiatorValve){
        coeffThermostaticRadiatorValve = Utility::randomDoubleNormal(-0.169, -0.038);
    }
    return coeffThermostaticRadiatorValve;
}

double Model_HeatingSetpoint::coeffCentralHeatingHoursReported(double centralHeatingHoursReported){
    double coeffCentralHeatingHoursReported = centralHeatingHoursReported*Utility::randomDoubleNormal(0.069, 0.006);
    return coeffCentralHeatingHoursReported;
}

double Model_HeatingSetpoint::coeffRegularHeatingPattern(bool regularHeatingPattern){
    double coeffRegularHeatingPattern = 0;
    if (regularHeatingPattern){
        coeffRegularHeatingPattern = Utility::randomDoubleNormal(1.189, 0.105);
    }
    return coeffRegularHeatingPattern;
}

double Model_HeatingSetpoint::coeffHouseHoldSize(int populationSize){
    double coeffHouseHoldSize = populationSize*Utility::randomDoubleNormal(0.250, 0.027);
    return coeffHouseHoldSize;
}

double Model_HeatingSetpoint::coeffHouseHoldIncome(double houseHoldIncome){
    // The initial model was calibrated on £.
    // The conversion to € is approximative and based on the change rate of November 2015
    // £1 = 1.4€
    int cat = 0;
    if (houseHoldIncome <= 7){
        cat = 0;
    }
    else if (houseHoldIncome > 7 && houseHoldIncome <= 15){
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
    else if (houseHoldIncome > 133 && houseHoldIncome <= 1000){
        cat = 6;
    }
    else {
        cat = Utility::randomInt(0,6);
    }
    double coeffHouseHoldIncome = cat*Utility::randomDoubleNormal(0.084, 0.020);
    return coeffHouseHoldIncome;
}

double Model_HeatingSetpoint::coeffAge(int age){
    double coeffAge = 0;
    if (age <= 5){
        coeffAge = Utility::randomDoubleNormal(0.495, 0.047);
    }
    else if (age > 5 && age <= 18){
        coeffAge = Utility::randomDoubleNormal(0.219, 0.024);
    }
    else if (age > 64 && age <= 74){
        coeffAge = Utility::randomDoubleNormal(0.370, 0.049);
    }
    else if (age > 74){
        coeffAge = Utility::randomDoubleNormal(0.585, 0.052);
    }
    else{}
    return coeffAge;
}

double Model_HeatingSetpoint::coeffTenureType(std::string tenureType){
    double coeffTenureType = 0;
    // OwnerOccupier, PrivatelyRented, CouncilTenant, HousingAssociation, Unknown
    double prob = Utility::randomDouble(0,1);
    if (tenureType == "Unknown"){ //http://www.insee.fr/fr/ffc/tef/tef2010/T10F072/T10F072.pdf
        if (prob < 57.8){
            tenureType = "OwnerOccupied";
        }
        else if (prob >= 57.8 && prob < 97){
            tenureType = "PrivatelyRented";
        }
        else{
            if (Utility::randomBool(0.5)){
                tenureType = "CouncilTenant";
            }
            else {
                tenureType = "HousingAssociation";
            }
        }
    }
    if (tenureType == "OwnerOccupied"){
    }
    else if (tenureType == "PrivatelyRented"){
        coeffTenureType = Utility::randomDoubleNormal(0.940, 0.063);
    }
    else if (tenureType == "CouncilTenant"){
        coeffTenureType = Utility::randomDoubleNormal(1.374, 0.076);
    }
    else if (tenureType == "HousingAssociation"){
        coeffTenureType = Utility::randomDoubleNormal(0.448, 0.054);
    }
    return coeffTenureType;
}

double Model_HeatingSetpoint::coeffTypology(std::string typology){
    double coeffTypology = 0;
    // Unknown, DetachedHouse, Semi-Detached , TerracedHouse, NotAHouse
    if (typology == "Unknown"){
        if (Utility::randomBool(0.5)){
            typology = "DetachedHouse";
        }
        else {
            typology = "Semi-Detached";
        }
    }
    else if (typology == "DetachedHouse"){
    }
    else if (typology == "Semi-Detached"){
        coeffTypology = Utility::randomDoubleNormal(0.694, 0.051);
    }
    else if (typology == "TerracedHouse"){
        coeffTypology = Utility::randomDoubleNormal(0.607, 0.035);
    }
    else if (typology == "NotAHouse"){
        coeffTypology = Utility::randomDoubleNormal(0.541, 0.045);
    }
    return coeffTypology;
}

double Model_HeatingSetpoint::coeffMainFuel(std::string mainFuel){
    double coeffMainFuel = 0;
    // Unknown, Electricity, Gas, Other
    double prob = Utility::randomDouble(0.0,1.0);
    // France, 2013, ADEME, Modes de chauffage dans l'habitat
    if (mainFuel == "Unknown"){
        if (prob <= 0.44){
            mainFuel = "Gas";
        }
        if (prob > 0.44 && prob <= 0.78){
            mainFuel = "Electricity";
        }
        else {
            mainFuel = "Other";
        }
    }
    else if (mainFuel == "Gas"){
        coeffMainFuel = Utility::randomDoubleNormal(-0.564, 0.47);
    }
    else if (mainFuel == "Electricity"){
        coeffMainFuel = Utility::randomDoubleNormal(1.008, 0.154);
    }
    else {}
    return coeffMainFuel;
}

double Model_HeatingSetpoint::coeffAdditionalFuel(std::string additionalFuel){
    double coeffAdditionalFuel = 0;
    // Unknown, None, Electricity, Other
    double prob = Utility::randomDouble(0.0,1.0);
    // France, 2013, ADEME, Modes de chauffage dans l'habitat
    if (additionalFuel == "Unknown"){
        if (prob <= 0.8){
            additionalFuel = "None";
        }
        if (prob > 0.8 && prob <= 0.9){
            additionalFuel = "Electricity";
        }
        else {
            additionalFuel = "Other";
        }
    }
    else if (additionalFuel == "Electricity"){
        coeffAdditionalFuel = Utility::randomDoubleNormal(-0.195, 0.044);
    }
    else if (additionalFuel == "Other"){
        coeffAdditionalFuel = Utility::randomDoubleNormal(-1.016, 0.227);
    }
    else {}
    return coeffAdditionalFuel;
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
    double coeffYearOfConstruction = cat*Utility::randomDoubleNormal(0.042, 0.010);
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
    double coeffRoofInsulationThickness = cat*Utility::randomDoubleNormal(0.125, 0.008);
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
    double coeffExtendOfDoubleGlazing = cat*Utility::randomDoubleNormal(0.188, 0.015);
    return coeffExtendOfDoubleGlazing;
}

double Model_HeatingSetpoint::coeffWallUValue(double wallUValue){
    int cat = 0;
    if (wallUValue <= 0.4 ){
        cat = 0;
    }
    else if (wallUValue > 0.4 && wallUValue <= 0.6){
        cat = 1;
    }
    else if (wallUValue > 0.6 && wallUValue <= 1.6){
        cat = 2;
    }
    else if (wallUValue > 1.6){
        cat = 3;
    }
    double coeffWallUValue = cat*Utility::randomDoubleNormal(0.076, 0.016);
    return coeffWallUValue;
}

//-------Dynamic-------//

double Model_HeatingSetpoint::inZone(double temperatureSetpointBase, double dailyTemperature, double coeffOutdoorTemperature, double coeffOutdoorTemperature2){
    double heatingSetpointState = temperatureSetpointBase + coeffOutdoorTemperature*dailyTemperature + coeffOutdoorTemperature2*pow(dailyTemperature,2);
    return heatingSetpointState;
}


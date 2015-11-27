#ifndef MODEL_HEATINGSETPOINT_H_INCLUDED
#define MODEL_HEATINGSETPOINT_H_INCLUDED

class Model_HeatingSetpoint {
public:
    Model_HeatingSetpoint();

    static double coeffLocalisation(std::string localisation);
    static double coeffRoomThermostat(bool RoomThermostat);
    static double coeffThermostatSetting(double thermostatSetting);
    static double coeffThermostaticRadiatorValve(bool thermostaticRadiatorValve);
    static double coeffCentralHeatingHoursReported(double centralHeatingHoursReported);
    static double coeffRegularHeatingPattern(bool regularHeatingPattern);
    static double coeffAutomaticTimer(bool automaticTimer);
    //static double coeffHouseHoldSize(int numberOfAgents());
    static double coeffHouseHoldIncome(double houseHoldIncome);
    static double coeffTenureType(std::string tenureType);
    static double coeffTypology(std::string typology);
    static double coeffGasCentralHeating(bool gasCentralHeating);
    static double coeffNonCentralHeating(bool nonCentralHeating);
    static double coeffElectricityIsMainFuel(bool electricityIsMainFuel);
    static double coeffAdditionalGasHeatingInLivingRoom(bool additionalGasHeatingInLivingRoom);
    static double coeffAdditionalElectricityHeatingInLivingRoom(bool additionalElectricityHeatingInLivingRoom);
    static double coeffAdditionalOtherHeatingInLivingRoom(bool additionalOtherHeatingInLivingRoom);
    static double coeffYearOfConstruction(double yearOfConstruction);
    static double coeffRoofInsulationThickness(double roofInsulationThickness);
    static double coeffExtendOfDoubleGlazing(std::string extendOfDoubleGlazing);
    static double coeffWallUValue(double wallUValue);


    /*
    double arrival(double state, double Tempint, bool gender);
    double intermediate(double state, double Tempint, bool gender);
    double departure(double state, double Tempint);
    */
private:
};

#endif // MODEL_HEATINGSETPOINT_H_INCLUDED

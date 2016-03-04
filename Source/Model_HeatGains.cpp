
#include <cmath>
#include "Model_HeatGains.h"

Model_HeatGains::Model_HeatGains() {
    maxI = 100;
}

/**
 * Radiant heat gains (all the sensible radiant stuff).
 * @return Radiant heat gains (all the sensible radiant stuff)
 */
double Model_HeatGains::getAllHeatGains() {
    return radiantHeatGains + convectiveHeatGains + dryRespiration + sweatEvaporation + latentRespirationHeatGains + vapourDiffusion;
}

double Model_HeatGains::getSweatEvaporation() const {
    return sweatEvaporation;
}

double Model_HeatGains::getDryRespiration() const {
    return dryRespiration;
}

double Model_HeatGains::getLatentRespirationHeatGains() const {
    return latentRespirationHeatGains;
}

double Model_HeatGains::getConvectiveHeatGains() const {
    return convectiveHeatGains;
}

double Model_HeatGains::getRadiantHeatGains() const {
    return radiantHeatGains;
}

double Model_HeatGains::getPmv() const {
    return pmv;
}

double Model_HeatGains::getPpd() const {
    return ppd;
}

/**
 * Compute Hc and Tcl iteratively subroutine
 * @param ta air temperature
 * @param icl clothing resistance
 * @param airVelocityAndBodyMovement Air velocity and body movement
 * @param surfaceAreaOfClothing ratio of clothed to nude body area
 * @param metabolicRate Metabolic Rate
 * @param externalWork external work
 * @param actualMeanRadiantTemperature mean radiant temperature
 */
void Model_HeatGains::aHCTCLcalc(double ta, double icl, double airVelocityAndBodyMovement, double surfaceAreaOfClothing, double metabolicRate, double externalWork, double actualMeanRadiantTemperature) {
    double ata = ta + 273.15;
    double counter = 0.0;
    double stcl = ata + (35.5 - ta) / (3.5 * (6.45 * icl + 0.1));
    double converge = 0.00015;
    double fv = 12.1 * sqrt(airVelocityAndBodyMovement);
    double p1 = icl * surfaceAreaOfClothing;
    double p2 = p1 * 3.96;
    double p3 = p1 * 100;
    double p4 = p1 * ata;
    double p5 = 308.7 - 0.028 * (metabolicRate - externalWork) + p2 * pow(actualMeanRadiantTemperature / 100, 4);
    xn = stcl / 100;
    double xf = xn / 2;
    while (std::abs(xn - xf) > converge && counter != maxI) {
        hc = 2.38 * pow(std::abs(100 * xf - ata), 0.25);
        if (hc < fv) {
            hc = fv;
        }
        xn = (p5 + p4 * hc - p2 * pow(xf, 4)) / (100 + p3 * hc);
        xf = (xf + xn) / 2;
        counter++;
    }
    tcl = 100 * xn - 273;
}

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
void Model_HeatGains::calculate(double metabolicRate, double reativeHumidity, double meanRadiantTemperature, double externalWork, double ta, double clo, double airVelocity) {
    double actualMeanRadiantTemperature = meanRadiantTemperature + 273.15;
    double partialWaterPressure = computePaCIBSEGuideC(actualMeanRadiantTemperature, reativeHumidity);
    double met = metabolicRate / 58.15;
    double airVelocityAndBodyMovement = airVelocity;
    if (met > 1) {
        airVelocityAndBodyMovement = airVelocity + 0.3 * (met - 1);
    }
    double icl = clo * .155;
    double surfaceAreaOfClothing = 1.05 + .645 * icl;
    if (icl < .078) {
        surfaceAreaOfClothing = 1 + 1.29 * icl;
    }
    aHCTCLcalc(ta, icl, airVelocityAndBodyMovement, surfaceAreaOfClothing, metabolicRate, externalWork, actualMeanRadiantTemperature);
    double TS = 0.303 * exp(-0.036 * metabolicRate) + 0.028;
    // vapourDiffusion + sweatEvaporation + latentRespirationHeatGains  = Vapour diffusion + sweat evaporation + latent respiration heat gains (all the latent stuff)
    vapourDiffusion = (3.05 * pow(10, -3)) * (5733 - 6.99 * (metabolicRate - externalWork) - partialWaterPressure);
    sweatEvaporation = 0;
    if ((metabolicRate - externalWork) > 58.15) {
        sweatEvaporation = 0.42 * (metabolicRate - externalWork - 58.15);
    }
    latentRespirationHeatGains = 1.7 * 0.00001 * metabolicRate * (5867 - partialWaterPressure);
    // dryRespiration + convectiveHeatGains = Dry respiration + convective heat gains (all the sensible convective stuff)
    dryRespiration = 0.0014 * metabolicRate * (34 - ta);
    radiantHeatGains = 3.96 * surfaceAreaOfClothing * (pow(xn, 4) - pow(actualMeanRadiantTemperature / 100, 4));
    convectiveHeatGains = surfaceAreaOfClothing * hc * (tcl - ta);
    pmv = TS * (metabolicRate - externalWork - vapourDiffusion - sweatEvaporation - latentRespirationHeatGains - dryRespiration - radiantHeatGains - convectiveHeatGains);

    ppd = 100 - 95 * exp(-0.03353 * pow(pmv, 4) - 0.2179 * pow(pmv, 2));
    //return pmv, ppd, radiantHeatGains, convectiveHeatGains;
}///check ppd for curve shape

/*
 ** Compute Pa (from CIBSE Guide C)
 */
double Model_HeatGains::computePaCIBSEGuideC(double actualMeanRadiantTemperature, double reativeHumidity) const {
    double Ps = pow(10, (30.59051 - 8.2 * log10(actualMeanRadiantTemperature) + 2.4804 * pow(10.0, -3.0) * actualMeanRadiantTemperature - (3142.31 / actualMeanRadiantTemperature)));
    double Pv = reativeHumidity / 100.0 * Ps;
    return Pv * 1000.0;
}


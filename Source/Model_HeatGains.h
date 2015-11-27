/*
 * File:   HeatGain.h
 * Author: jake
 *
 * Created on September 15, 2013, 5:03 PM
 */

#ifndef HEATGAIN_H
#define HEATGAIN_H

class Model_HeatGains {
public:
    Model_HeatGains();

    void calculate(
        double metabolicRate,
        double reativeHumidity,
        double meanRadiantTemperature,
        double externalWork,
        double ta,
        double clo,
        double airVelocity);
    double getAllHeatGains();
    double getSweatEvaporation() const;
    double getDryRespiration() const;
    double getLatentRespirationHeatGains() const;
    double getConvectiveHeatGains() const;
    double getRadiantHeatGains() const;
    double getPmv() const;
    double getPpd() const;

private:
    void aHCTCLcalc(
        double ta,
        double icl,
        double airVelocityAndBodyMovement,
        double surfaceAreaOfClothing,
        double metabolicRate,
        double externalWork,
        double actualMeanRadiantTemperature);

    double computePaCIBSEGuideC(double actualMeanRadiantTemperature, double reativeHumidity) const;

    double ppd;
    double pmv;
    double radiantHeatGains;
    double convectiveHeatGains;
    double latentRespirationHeatGains;
    double dryRespiration;
    double sweatEvaporation;
    double vapourDiffusion;
    int maxI;
    double hc;
    double tcl;
    double xn;

};

#endif  /* HEATGAIN_H */

/*
 * File:   Model_RandomWeibull.cpp
 * Author: jake
 *
 * Created on September 28, 2013, 6:34 PM
 */
#include <cmath>
#include "Utility.h"
#include "Model_RandomWeibull.h"

Model_RandomWeibull::Model_RandomWeibull() {
}

double Model_RandomWeibull::randomWeibull(double scale, double shape) {
    // Draws a random number from a Weibull distribution,
    // proceeding by inversion of the Weibull cdf, defined as
    // F(x) = 1 - exp(-(x/scale)^shape)
    //double x = randomUniform(0.f,1.f);
    //double y = scale*pow(-log(1.f-random),1.f/shape);

    double random = randomDouble();
    return scale * pow(-log(1.f-random), 1.f / shape);
}

double Model_RandomWeibull::randomDouble() {
    return randomDouble(0.0,1.0);
}

double Model_RandomWeibull::randomDouble(double min, double max){
    return Utility::randomDouble(min, max);
}

double Model_RandomWeibull::probability(double m){
    double result = 0;
    if(m < log(FLT_MAX)){
        result = std::exp (m);
    }
    return result / (1.f + result);
}

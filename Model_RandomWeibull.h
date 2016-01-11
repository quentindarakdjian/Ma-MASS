/*
 * File:   Model_RandomWeibull.h
 * Author: jake
 *
 * Created on September 28, 2013, 6:34 PM
 */

#ifndef MODEL_RANDOMWEIBULL_H
#define	MODEL_RANDOMWEIBULL_H

#include <cfloat>

class Model_RandomWeibull {
public:
    Model_RandomWeibull();

    static double randomWeibull(double scale, double shape);
    static double randomDouble();
    static double randomDouble(double min, double max);
    static double probability(double m);

private:
};

#endif	/* MODEL_RANDOMWEIBULL_H */


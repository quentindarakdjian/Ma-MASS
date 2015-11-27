/*
 * File:   Model_lights.h
 * Author: jake
 *
 * Created on 26 October 2013, 23:13
 */

#ifndef MODEL_LIGHTS_H
#define	MODEL_LIGHTS_H

class Model_Lights {
public:
    Model_Lights();
    
    bool arrival(bool state, double Lumint);
    bool intermediate(bool state, double Lumint);
    bool departure(bool state, double Lumint, double futureDuration);
private:
};

#endif	/* MODEL_LIGHTS_H */

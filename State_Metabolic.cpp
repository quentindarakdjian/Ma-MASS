#include "State_Metabolic.h"

State_Metabolic::State_Metabolic()
{
    //ctor
            id = 7;// Metabolic
            metabolicRate = 93;
            clo = 1;
            activity = "Metabolic";
    location = getLocationFromActivty(&activity);

}

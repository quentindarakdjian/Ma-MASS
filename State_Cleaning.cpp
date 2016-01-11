#include "State_Cleaning.h"

State_Cleaning::State_Cleaning()
{
    //ctor
            id = 5;
            metabolicRate = 116;
            clo = 1;
            activity = "Cleaning";
            location = getLocationFromActivty(&activity);

}

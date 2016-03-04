#include "State_Cooking.h"

State_Cooking::State_Cooking()
{
    //ctor
            id = 4; //Cooking
            metabolicRate = 116;
            clo = 1;
            activity = "Cooking";
            location = getLocationFromActivty(&activity);

}

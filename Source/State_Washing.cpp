#include "State_Washing.h"

State_Washing::State_Washing()
{
    //ctor
            id = 6;// Other Washing
            metabolicRate = 116;
            clo = 0;
            activity = "Washing";
    location = getLocationFromActivty(&activity);

}

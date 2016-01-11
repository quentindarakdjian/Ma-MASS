#include "State_Sleep.h"

State_Sleep::State_Sleep()
{
    //ctor
    id = 0; // sleep
    metabolicRate = 46;
    clo = 2.55;
    activity = "Sleep";
    location = getLocationFromActivty(&activity);
}

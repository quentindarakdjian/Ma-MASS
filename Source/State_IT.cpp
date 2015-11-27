#include "State_IT.h"

State_IT::State_IT()
{
    //ctor
    id = 3; // IT
    metabolicRate = 116;
    clo = 1;
    activity = "IT";
    location = getLocationFromActivty(&activity);

}

#include "State_Passive.h"

State_Passive::State_Passive()
{
    //ctor
    id = 1; //passive
    metabolicRate = 58;
    clo = 1;
    activity = "Passive";
    location = getLocationFromActivty(&activity);
}

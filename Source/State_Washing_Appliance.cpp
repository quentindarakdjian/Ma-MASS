#include "State_Washing_Appliance.h"

State_Washing_Appliance::State_Washing_Appliance()
{
    //ctor
    id = 8; // washing appliance
    metabolicRate = 116;
    clo = 1;
    activity = "WashingAppliance";
    location = getLocationFromActivty(&activity);
}

#include "State_Audio_Visual.h"

State_Audio_Visual::State_Audio_Visual()
{
    id = 2;
    metabolicRate = 70;
    clo = 1;
    activity = "AudioVisual";
    location = getLocationFromActivty(&activity);
    //ctor
}

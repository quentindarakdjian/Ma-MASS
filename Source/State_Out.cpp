#include "State_Out.h"

State_Out::State_Out()
{
    //ctor
            id = 9; // out
            metabolicRate = 0;
            clo = 1;
            activity = "Out";
            location = getLocationFromActivty(&activity);

}

bool State_Out::hasZone() const{
    return false;
}

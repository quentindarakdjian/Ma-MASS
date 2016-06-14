#ifndef STATE_H
#define STATE_H
#include <string>
#include "Zone.h"




class State{
    public:
        State();
        virtual ~State(); // The destructor is virtual because the State class is inherited by the 11 sub-states.
        //The desctructed object is then the child.

        int getId() const;
        double getMetabolicRate() const;
        double getClo() const;
        std::vector<std::string> getLocationFromActivty(const std::string *activity);
        std::vector<std::string> getLocation() const;
        std::string getActivity() const;
        std::vector<Zone*> getZonesPtr() const;
		    void addState(State s);
        virtual bool hasState(const int stateID);
        State getNextState();
        void addZonePtr(Zone* zoneptr);

    protected:
        int id;
        double metabolicRate;
        double clo;
        std::vector<std::string> location;
        std::string activity;
        std::vector<Zone*> zones;
    private:

};

#endif // STATE_H

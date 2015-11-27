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
        std::string getLocationFromActivty(const std::string *activity);
        std::string getLocation() const;
        std::string getActivity() const;
        Zone* getZonePtr() const;
		void addState(State s);
        virtual bool hasState(const int stateID);
        State getNextState();
        void setZonePtr(Zone* zoneptr);
    protected:
        int id;
        double metabolicRate;
        double clo;
        std::string location;
        std::string activity;
        Zone* zone;
    private:

};

#endif // STATE_H

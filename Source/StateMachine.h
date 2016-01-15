#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <vector>
#include "State.h"


class StateMachine
{
    public:
      StateMachine();

      void addState(State s);
      State transistionTo(const int stateID);
      bool hasState(const int stateID) ;
      State getPendingState();
    protected:
    private:

      //int initialStateLocation;
      State pendingState;
      std::vector<State> states;
};

#endif // STATEMACHINE_H

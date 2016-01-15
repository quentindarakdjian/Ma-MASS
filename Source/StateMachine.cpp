#include <iostream>
#include "StateMachine.h"



StateMachine::StateMachine(){
}

void StateMachine::addState(State s){
	states.push_back(s);
}

State StateMachine::transistionTo(const int stateID){
    if(!hasState(stateID)){
        for(auto s : states){
            if(s.hasState( stateID)){
                //std::cout << "id: " << stateID << " " << s.getNextState().getLocation() << std::endl;
                return s.getNextState();
            }
        }
    }
    return getPendingState();
}

bool StateMachine::hasState(const int stateID){
    bool b = false;
    for(auto s : states){
        if(s.getId() == stateID){
            pendingState = s;
            b = true;
            break;
        }
    }
    return b;
}

State StateMachine::getPendingState(){
    return pendingState;
}

#include "../include/TransitionTable.h"

TransitionTable::TransitionTable()
{
    //ctor
}

TransitionTable::~TransitionTable()
{
    //dtor
}

bool TransitionTable::addTransition(int from, int to, char c) {
    if (stateTable.size() <= from || stateTable.size() <= to) {
        return false;
    }

    stateTable[from].addTransition(to, c);
    return true;
}

void TransitionTable::addState() {
    State newState;
    stateTable.push_back(newState);
}

void TransitionTable::setStartState(int startState) {
    this->startState = startState;
}

void TransitionTable::setFinalState(int finalState) {
    this->finalState = finalState;
}

#include "State.h"

State::State()
{
    //ctor
}

State::~State()
{
    //dtor
}


void State::addTransition(string states, char c) {

    if (transitions.find(c) == transitions.end()) {

        transitions[c] = states;
    } else {
        transitions[c] += "," + states;
    }
}

void State::addNumberToTransitions(int number) {

    for (auto& x : transitions){

        x.second = addNumber(x.second, number);
    }
}

unordered_map<char, string >State::getInputsWithTranstions() {

    return transitions;
}

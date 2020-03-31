#include "../include/NFA.h"

NFA::NFA()
{
    State s1,s2;
    stateTable.push_back(s1);
    stateTable.push_back(s2);
    startState = 0;
    finalState = 1;
    stateTable[0].addTransition("1", ' ');
}

NFA::~NFA()
{
    //dtor

}

NFA::NFA(char c)
{
    State s1,s2;
    stateTable.push_back(s1);
    stateTable.push_back(s2);
    startState = 0;
    finalState = 1;
    stateTable[0].addTransition("1", c);

}

void NFA::concatenate(NFA nfa)
{

    int newStateStart = finalState;
    for (int i = 0; i < nfa.stateTable.size(); i++)
    {
        nfa.stateTable[i].addNumberToTransitions(newStateStart);
    }

    for (auto& x : nfa.stateTable[0].getInputsWithTranstions())
    {
        stateTable[finalState].addTransition(x.second, x.first);
    }
    for (int i = 1; i < nfa.stateTable.size(); i++)
    {
        stateTable.push_back(nfa.stateTable[i]);
    }
    finalState = finalState + nfa.stateTable.size() - 1;


}
void NFA::closure()
{

    stateTable[finalState].addTransition(to_string(startState), ' ');
    State newFinalState;
    stateTable[finalState].addTransition(to_string(finalState + 1),' ');
    stateTable.push_back(newFinalState);
    finalState = finalState + 1;
    for (int i = 0; i < stateTable.size(); i++)
    {
        stateTable[i].addNumberToTransitions(1);
    }
    State newstartState;
    newstartState.addTransition("1," + to_string(finalState + 1), ' ');
    vector<State> newStateTable;
    newStateTable.push_back(newstartState);
    for (int i = 0; i < stateTable.size(); i++)
    {
        newStateTable.push_back(stateTable[i]);
    }
    finalState = finalState + 1;
    stateTable = newStateTable;
}
void NFA::positiveClosure()
{
    stateTable[finalState].addTransition(to_string(startState), ' ');
    State newFinalState;
    stateTable[finalState].addTransition(to_string(finalState + 1), ' ');
    stateTable.push_back(newFinalState);
    finalState = finalState + 1;
}

void NFA::unionn(NFA nfa)
{
    int newStateStartnumber = finalState + 2;
    for (int i = 0; i < nfa.stateTable.size(); i++)
    {
        nfa.stateTable[i].addNumberToTransitions(newStateStartnumber);
    }

    for (int i = 0; i < stateTable.size(); i++)
    {
        stateTable[i].addNumberToTransitions(1);
    }
    State newStartState, newFinalState;
    newStartState.addTransition("1," + to_string(newStateStartnumber), ' ');
    int newFinalStateNumber = stateTable.size() + nfa.stateTable.size() + 1;

    stateTable[finalState].addTransition(to_string(newFinalStateNumber), ' ');
    nfa.stateTable[nfa.finalState].addTransition(to_string(newFinalStateNumber), ' ');

    vector<State> newStateTable;
    newStateTable.push_back(newStartState);
    for (int i = 0; i < stateTable.size(); i++)
    {
        newStateTable.push_back(stateTable[i]);
    }
    for (int i = 0; i < nfa.stateTable.size(); i++)
    {
        newStateTable.push_back(nfa.stateTable[i]);
    }
    newStateTable.push_back(newFinalState);
    stateTable = newStateTable;
    finalState = newFinalStateNumber;
}
void NFA::toString()
{
    for (int i = 0; i < stateTable.size(); i++)
    {
        for (auto y : stateTable[i].getInputsWithTranstions())
        {
            cout << i << " " << y.first << " " << y.second << endl;
        }
    }
    cout << startState << " " << finalState << endl;
}

vector<State>NFA::getStateTable() {
    return stateTable;
}
void NFA::addNumberToTransitions(int number) {
    for (int i = 0; i < stateTable.size(); i++) {
        stateTable[i].addNumberToTransitions(number);
    }
}

void NFA::addTransition(int from, int to, char input) {
    stateTable[from].addTransition(to_string(to), input);
}

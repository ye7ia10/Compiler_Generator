#include "../header/NFA.h"

NFA::NFA()
{
    //ctor
}

NFA::~NFA()
{
    //dtor
}

NFA::NFA(char c)
{

    stateTable.resize(2);
    startState = 0;
    finalState = 1;
    stateTable[0][c] = "1";
}

void NFA::concatenate(NFA nfa)
{

    int newStateStart = finalState; /* this is the start state of nfa which is 0 */
    for (int i = 0; i < nfa.stateTable.size(); i++)
    {
        for (auto& x : nfa.stateTable[i])
        {
            x.second = addNumber(x.second, newStateStart);
        }
    }
    for (auto& x : nfa.stateTable[0])
    {
        if (stateTable[finalState].find(x.first) == stateTable[finalState].end())
        {
            stateTable[finalState][x.first] = x.second;
        }
        else
        {
            stateTable[finalState][x.first] = stateTable[finalState][x.first] + ',' + x.second;
        }
    }
    for (int i = 1; i < nfa.stateTable.size(); i++)
    {
        stateTable.push_back(nfa.stateTable[i]);
    }
    finalState = finalState + nfa.stateTable.size() - 1;


}
void NFA::closure()
{
    if (stateTable[finalState].find(' ') == stateTable[finalState].end())
    {
        stateTable[finalState][' '] = to_string(startState);
    }
    else
    {
        stateTable[finalState][' '] = stateTable[finalState][' '] + ',' + to_string(startState);
    }
    unordered_map<char, string> newFinalState;
    stateTable[finalState][' '] = stateTable[finalState][' '] + ',' + to_string(finalState + 1);
    stateTable.push_back(newFinalState);
    finalState = finalState + 1;
    for (int i = 0; i < stateTable.size(); i++)
    {
        for (auto& x : stateTable[i])
        {
            x.second = addNumber(x.second, 1);
        }
    }
    unordered_map<char, string> newstartState;
    newstartState[' '] = "1," + to_string(finalState + 1);
    vector<unordered_map<char, string> > newStateTable;
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
    if (stateTable[finalState].find(' ') == stateTable[finalState].end())
    {
        stateTable[finalState][' '] = to_string(0);
    }
    else
    {
        stateTable[finalState][' '] = stateTable[finalState][' '] + ',' + to_string(0);
    }

    unordered_map<char, string> newFinalState;
    stateTable[finalState][' '] = stateTable[finalState][' '] + ',' + to_string(finalState + 1);
    stateTable.push_back(newFinalState);
    finalState = finalState + 1;


}

void NFA::unionn(NFA nfa)
{
    int newStateStartnumber = finalState + 2;
    for (int i = 0; i < nfa.stateTable.size(); i++)
    {
        for (auto& x : nfa.stateTable[i])
        {
            x.second = addNumber(x.second, newStateStartnumber);
        }
    }

    for (int i = 0; i < stateTable.size(); i++)
    {
        for (auto& x : stateTable[i])
        {
            x.second = addNumber(x.second, 1);
        }
    }
    unordered_map<char, string> newStartState, newFinalState;
    newStartState[' '] = "1," + to_string(newStateStartnumber);
    int newFinalStateNumber = stateTable.size() + nfa.stateTable.size() + 1;
    if(stateTable[finalState].find(' ') == stateTable[finalState].end())
    {
        stateTable[finalState][' '] = to_string(newFinalStateNumber);
    }
    else
    {
        stateTable[finalState][' '] = stateTable[finalState][' '] + '.' + to_string(newFinalStateNumber);
    }
    if(nfa.stateTable[nfa.finalState].find(' ') == nfa.stateTable[nfa.finalState].end())
    {
        nfa.stateTable[nfa.finalState][' '] = to_string(newFinalStateNumber);
    }
    else
    {
        nfa.stateTable[nfa.finalState][' '] = nfa.stateTable[nfa.finalState][' '] + '.' + to_string(newFinalStateNumber);
    }
    vector<unordered_map<char, string> > newStateTable;
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
        for (auto y : stateTable[i])
        {
            cout << i << " " << y.first << " " << y.second << endl;
        }
    }
    cout << startState << " " << finalState << endl;
}

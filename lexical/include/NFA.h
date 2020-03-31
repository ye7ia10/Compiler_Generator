#ifndef NFA_H
#define NFA_H
#include "stringParsing.h"
#include "State.h"
#include<bits/stdc++.h>
using namespace std;
class NFA
{
    public:
        NFA();
        int priority = 0;
        NFA(char c);
        virtual ~NFA();
        int startState = 0;
        int finalState = 0;
        string name;
        void concatenate(NFA nfa);
        void closure();
        void positiveClosure();
        void unionn(NFA nfa);
        void toString();
        vector<State> getStateTable();
        void addNumberToTransitions(int number);
        void addTransition(int from, int to, char c);
    protected:

    private:
        vector<State> stateTable;
};

#endif // NFA_H

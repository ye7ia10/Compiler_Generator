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

        NFA(char c);
        virtual ~NFA();
        void concatenate(NFA nfa);
        void closure();
        void positiveClosure();
        void unionn(NFA nfa);
        vector<State> getStateTable();
        void addNumberToTransitions(int number);
        void addTransition(int from, int to, char c);
        int getPriority();
        void setPriority(int prioriy);
        string getName();
        void setName(string name);
        int getFinalState();
        void toString();
        void setFinalState(int state);
    protected:

    private:
        vector<State> stateTable;
        int startState = 0;
        int finalState = 0;
};

#endif // NFA_H

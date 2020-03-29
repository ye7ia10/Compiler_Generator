#ifndef TRANSITIONTABLE_H
#define TRANSITIONTABLE_H
#include <bits/stdc++.h>

class TransitionTable
{
    public:
        TransitionTable();
        virtual ~TransitionTable();
        /* add transition from state (from) to state (to) under input c.
 return true if the two */
        bool addTransition(int from, int to, char c);
        /* set start state of to the transition table */
        void setStartState(int state);
        /* set final state of to the transition table */
        void setFinalState(int state);
        /* add state to the existance table*/
        void addState();
    protected:

    private:
    vector<unordered_map<char, string> > stateTable;
    int startState;
    int finalState;
};

#endif // TRANSITIONTABLE_H

#ifndef NFA_H
#define NFA_H
#include "stringParsing.h"
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
        vector<unordered_map<char, string> > stateTable;
        void concatenate(NFA nfa);
        void closure();
        void positiveClosure();
        void unionn(NFA nfa);
        void toString();
    protected:

    private:
};

#endif // NFA_H

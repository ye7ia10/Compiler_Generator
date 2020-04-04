#ifndef STATE_H
#define STATE_H
#include "stringParsing.h"
#include <bits/stdc++.h>
using namespace std;
class State
{
    public:
        State();
        virtual ~State();
        /* add transition to theses state in input (c) */
        void addTransition(string states, char c);
        /* add constant number to the transition of this state */
        void addNumberToTransitions(int number);
        unordered_map<char, string > getInputsWithTranstions();
        /* get transition of state in specific input */
        string getTranstion(char c);
        vector<char>getInputTags();

        void setFinalState(bool isFinal);
        bool isFinalState();
        void setName(string name);
        string getName();
        void setPriority(int priority);
        int getPriority();

    protected:

    private:
    unordered_map<char, string > transitions;
    int priority;
    bool isFinal;
    string name;
};

#endif // STATE_H

#ifndef CONVERSION_DFA_H
#define CONVERSION_DFA_H

#include <iostream>
#include <vector>
#include <sstream>
#include <stack>
#include <set>
#include <map>
#include <string>
#include <bits/stdc++.h>
#include "State.h"
using namespace std;

class conversionDfa
{

private:
    int numStates;
    int n; //number of dfa states
    int m; //number of inputs
    vector<State> nfaTransitions;
    vector<string> inputsTags;

    unordered_map<string, int> dfaStatesMap;
    vector<string> dfaStates;
    vector<vector<string>> dfaTransitions;
    vector<vector<string> > pi[2];
    vector<bool> finalStatesMark;
    map<string, int> dfaStatesFinal;



    string getEClosure(string states);
    string applyInput(string currState, string currInput);
    vector<string> defaultRow();
    void convertToDfa();

    void SetUpVars();
    bool checkMatching (string state1, string state2);
    int getPartitionIdx(string state);
    bool CheckIfDone ();
    void Minimize ();
    void getTheMinimizedDFATable ();
    vector<vector<string>> changeConventionTransitions();
    vector<string> changeConventionStates();
    bool sameKind(string state1, string state2);



public:
    conversionDfa(
        vector<State> nfaTransitions, vector<string> inputsTags);
    vector<vector<string>> getTransitionTable();
    vector<string> getDfaStates();

    map<string, string> mapStates;

    map<string, vector<string>> groupingFinalStates();
    map<string, int> dfaStatesFinalNew;

};

#endif

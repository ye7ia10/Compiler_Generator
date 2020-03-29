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

using namespace std;

class conversionDfa
{

private:
    int numStates;
    int n; //number of dfa states
    int m; //number of inputs
    vector<string> initialStates;
    vector<string> finalStates;
    vector<vector<string>> nfaTransitions;
    vector<string> inputsTags;
    vector<string> statesTags;
    vector<string> dfaStates;
    vector<vector<string>> dfaTransitions;
    vector<vector<string> > pi[2];
    vector<bool> finalStatesMark;
    vector<string>finalStatesNameOrdered;
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
    conversionDfa(int numStates, vector<string> initialStates, vector<string> finalStates,
                  vector<vector<string>> nfaTransitions, vector<string> inputsTags, vector<string> statesTags
                  , vector<string>finalStatesNameOrdered, unordered_map<int, int>finalStatesPriority);
    vector<vector<string>> getTransitionTable();
    vector<string> getDfaStates();

    map<string, string> mapStates;
    unordered_map<int, int>finalStatesPriority;
    map<string, vector<string>> groupingFinalStates();
    map<string, int> dfaStatesFinalNew;

};

#endif

#ifndef LEXICAL_H
#define LEXICAL_H

#include "conversionDfa.h"
#include "RegularParser.h"
#include "stringParsing.h"
#include "readFile.h"
#include "Token.h"
using namespace std;

class LexicalAnalyzerGenerator
{

private:
    // NFA Variables
    int numStates;
    vector<string> inputsTags;
    vector<string> initialStates;
    vector<string> finalStates;
    vector<string> finalStatesOrdered;
    vector<string> finalStatesNamesOrdered;
    vector<State> nfaTransitions;
    unordered_map<int, int> finalStatesPriority;
    unordered_map<int, string> acceptanceNames;

    // DFA Variables
    vector<string> dfaStates;
    vector<vector<string>> dfaTransitions;
    map<string, vector<string>> finalStatesGroups;
    map<string, string> mapStates;
    map<string, int> dfaFinalStatesPriority;

    bool checkApplication(string currState, string input);
    string inputApplication(string currState, string input);
    bool checkFinalState(string state);
    string getName(string state);






public:
    LexicalAnalyzerGenerator();

    void NFAGenerator();
    void DFAMinizedGenerator();
    void printTest();
    void applyInput(string file);

    // Wakeel: Making sure that function work correctly
    void testFun(string curr, string input);
    vector<Token> tokenVec;

};

#endif

#include <iostream>
#include <vector>
#include <algorithm>
#include "LexicalAnalyzerGenerator.h"

using namespace std;

int main()
{

    /*vector<string> initialStates;
    vector<string> finalStates;
    vector<vector<string>> nfaTransitions;
    vector<string> inputsTags;
    vector<string> statesTags;

    RegularParser regularParser;

    vector<string> inputLines = readFile("test.txt");

    regularParser.parseFile(inputLines);

    int numStates = regularParser.table.size();

    initialStates.push_back("0");

    finalStates = regularParser.finalStates;

    cout << "final states size :" << finalStates.size() << " is " << finalStates[0] << endl;

    for (int i = 0; i < numStates; i++){
        statesTags.push_back(to_string(i));
    }


    inputsTags = regularParser.inputsTags;

    nfaTransitions = regularParser.table;

    vector<string> finalStatesOrdered = regularParser.finalStatesNameOrdered;


    conversionDfa dfa(numStates, initialStates, finalStates, nfaTransitions, inputsTags, statesTags
                      , regularParser.finalStatesNameOrdered, regularParser.finalStatesPriority);

    vector<vector<string>> dfaTransitions = dfa.getTransitionTable();
    vector<string> dfaStates = dfa.getDfaStates();

    cout << "input tags" << endl;
    for (int i = 0; i < inputsTags.size() ; i++){
        cout << inputsTags[i] << " " ;
    }
    cout << endl;
    cout << "nfa transitions :" << endl;
    for (int i = 0; i < nfaTransitions.size(); i++)
    {
        cout << i << ": ";
        for (int j = 0; j < nfaTransitions[i].size(); j++)
        {
            cout << nfaTransitions[i][j] + " ";
        }
        cout << "\n";
    }

    map<string, vector<string>> mapping = dfa.groupingFinalStates();


    std::map<std::string, vector<string> >::iterator it = mapping.begin();

    cout<<endl;
    cout<<"dfa final states with priority"<<endl;
    while (it != mapping.end())
    {
        vector<string> groupStates = it -> second;
        for (int j = 0; j < groupStates.size(); j++){
                if (dfa.mapStates.find(groupStates[j]) != dfa.mapStates.end()){
                    cout<< "Priority: "<< it -> first << " --> " << dfa.mapStates[groupStates[j]]<<endl;
                }
            }
            it++;
            cout<< endl;
    }
    cout<<endl;

    cout << "dfa states" << endl;
    for (int i = 0; i < dfaStates.size() ; i++){
        cout << dfaStates[i] << endl;
    }

    cout << "dfa transitions" << endl;
    for (int i = 0; i < dfaTransitions.size(); i++)
    {
        cout << dfaStates[i] + ": ";
        for (int j = 0; j < dfaTransitions[i].size(); j++)
        {
            cout << dfaTransitions[i][j] +       " ^^^  " + inputsTags[j]     + " | ";
        }
        cout << "\n";
    }*/

    LexicalAnalyzerGenerator lexicalAnalyzerGenerator;

    lexicalAnalyzerGenerator.NFAGenerator();

    lexicalAnalyzerGenerator.DFAMinizedGenerator();
    lexicalAnalyzerGenerator.printTest();


    ofstream myfile;
    myfile.open ("Output.txt");



    /* read input and apply lexical rule */
    lexicalAnalyzerGenerator.applyInput("input.txt");
    for (int i = 0; i < lexicalAnalyzerGenerator.tokenVec.size(); i++) {
        cout << lexicalAnalyzerGenerator.tokenVec[i].toString() << endl;
        myfile <<  lexicalAnalyzerGenerator.tokenVec[i].toString() << endl;
    }

    myfile.close();
    return 0;
}

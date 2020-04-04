#include "../include/LexicalAnalyzerGenerator.h"

LexicalAnalyzerGenerator::LexicalAnalyzerGenerator()
{

}

void LexicalAnalyzerGenerator::NFAGenerator()
{




    vector<string> inputLines = readFile("test.txt");
    RegularParser regularParser(inputLines);

    int numStates = regularParser.getTotalNFAStates().size();

    initialStates.push_back("0");

    nfaTransitions = regularParser.getTotalNFAStates();
    /* get all different input tags in the transition table and get names of final states wih there priority*/
    set<char>diffInput;
    for (int i = 0; i < nfaTransitions.size(); i++) {
            if (nfaTransitions[i].isFinalState()) {
                acceptanceNames[nfaTransitions[i].getPriority()] = nfaTransitions[i].getName();
            }
            for(auto it : nfaTransitions[i].getInputsWithTranstions())
            {

                if (diffInput.find(it.first) == diffInput.end() && it.first != ' ')
                {
                    diffInput.insert(it.first);
                    string str = "";
                    str += it.first;
                    inputsTags.push_back(str);
                }
            }
    }
    /* add epsilon transition */
    inputsTags.push_back(" ");

}

void LexicalAnalyzerGenerator::DFAMinizedGenerator()
{

    conversionDfa dfa(nfaTransitions, inputsTags);

    dfaTransitions = dfa.getTransitionTable();
    dfaStates = dfa.getDfaStates();
    finalStatesGroups = dfa.groupingFinalStates();
    mapStates = dfa.mapStates;
    dfaFinalStatesPriority = dfa.dfaStatesFinalNew;
}

bool LexicalAnalyzerGenerator::checkApplication(string currState, string input)
{

    auto it = find(inputsTags.begin(), inputsTags.end(), input);
    if (it == inputsTags.end())
    {
        return false;
    }
    int inputIndex = distance(inputsTags.begin(), it);

    if (dfaTransitions[stoi(currState)][inputIndex] != "-")
    {
        return true;
    }
    else
    {
        return false;
    }
}

string LexicalAnalyzerGenerator::inputApplication(string currState, string input)
{

    auto it = find(inputsTags.begin(), inputsTags.end(), input);
    int inputIndex = distance(inputsTags.begin(), it);

    return dfaTransitions[stoi(currState)][inputIndex];
}

bool LexicalAnalyzerGenerator::checkFinalState(string state)
{

    if (dfaFinalStatesPriority.find(state) != dfaFinalStatesPriority.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

string LexicalAnalyzerGenerator::getName(string state)
{

    int priority = dfaFinalStatesPriority[state];

    return acceptanceNames[priority];

}



void LexicalAnalyzerGenerator::printTest()
{

    cout << "input tags" << endl;
    for (int i = 0; i < inputsTags.size() ; i++)
    {
        cout << inputsTags[i] << " " ;
    }
    cout << endl;
    /*
    cout << "NFA transitions :" << endl;
    for (int i = 0; i < nfaTransitions.size(); i++)
    {
        cout << i << ": ";
        for (int j = 0; j < nfaTransitions[i].size(); j++)
        {
            cout << nfaTransitions[i][j] + " ";
        }
        cout << "\n";
    }
    */

    std::map<std::string, vector<string> >::iterator it = finalStatesGroups.begin();
    cout<<endl;
    cout<<"DFA final states with priority"<<endl;
    while (it != finalStatesGroups.end())
    {
        vector<string> groupStates = it -> second;
        for (int j = 0; j < groupStates.size(); j++)
        {
            if (mapStates.find(groupStates[j]) != mapStates.end())
            {
                cout<< "Priority: "<< it -> first << " --> " << mapStates[groupStates[j]]<<endl;
            }
        }
        it++;
        cout<< endl;
    }
    cout<<endl;

    cout << "DFA states" << endl;
    for (int i = 0; i < dfaStates.size() ; i++)
    {
        cout << dfaStates[i] << endl;
    }

    cout << "DFA transitions" << endl;
    for (int i = 0; i < dfaTransitions.size(); i++)
    {
        cout << dfaStates[i] + ": ";
        for (int j = 0; j < dfaTransitions[i].size(); j++)
        {
            cout << inputsTags[j] << " -> " << dfaTransitions[i][j] << " | ";
        }
        cout << "\n";
    }

    // Printing for testing get the priority of final states
    /*cout<<endl;
    cout<<"DFA final states with priority_2"<<endl;

    for (int i = 0; i < dfaStates.size(); i++){
        if (dfaFinalStatesPriority.find(dfaStates[i]) != dfaFinalStatesPriority.end()){

            cout<< "Final State: " << dfaStates[i] << " With Priority: " << dfaFinalStatesPriority[mapStates[dfaStates[i]]] << endl;
        }
         cout<< endl;
    }*/
}

void LexicalAnalyzerGenerator::testFun(string curr, string input)
{
    if (checkApplication(curr, input))
    {
        string nextState = inputApplication(curr, input);

        if (checkFinalState(nextState))
        {
            cout<<"D5l"<<endl;
        }
    }
}
void LexicalAnalyzerGenerator::applyInput(string file)
{

    vector<string>lines = readFile(file);
    int line = 1;
    bool errorInPreviousChar = false;
    for (int i = 0; i < lines.size(); i++)
    {

        vector<string> strings = splitByChar(lines[i], ' ');
        for (int j = 0; j < strings.size(); j++)
        {

            string currentState = "0";
            int lastMatched = -1;
            int sizee = 0;
            int lastMatchedSize = 0;
            string lastMatchedState;
            for (int k = 0; k < strings[j].size(); k++)
            {
                string input = "";
                input = input + strings[j][k];
                if (checkApplication(currentState, input) && k + 1 < strings[j].size())   /* matched but still can match other char */
                {
                    currentState = inputApplication(currentState, input);
                    sizee++;
                    if (checkFinalState(currentState))
                    {
                        lastMatched = k;
                        lastMatchedState = currentState;
                        lastMatchedSize = sizee;
                    }


                }
                else       /* last char or not matched */
                {
                    if (checkApplication(currentState, input))
                    {
                        currentState = inputApplication(currentState, input);
                        sizee++;
                        if (checkFinalState(currentState))
                        {
                            lastMatched = k;
                            lastMatchedState = currentState;
                            lastMatchedSize = sizee;
                        }
                    }
                    if (lastMatchedSize == 0)   /* no match happen */
                    {
                        if(errorInPreviousChar)
                        {
                            tokenVec.back().addCharToName(strings[j][k]);
                        }
                        else
                        {
                            errorInPreviousChar = true;
                            Token token(input, "", line, true);
                            tokenVec.push_back(token);
                        }
                    }
                    else      /* a match happen */
                    {
                        errorInPreviousChar = false;
                        string rule = getName(lastMatchedState);
                        Token token(strings[j].substr(lastMatched - lastMatchedSize + 1, lastMatchedSize), rule, line, false);
                        tokenVec.push_back(token);
                        k = lastMatched;
                    }
                    currentState = "0";
                    lastMatched = -1;
                    sizee = 0;
                    lastMatchedSize = 0;
                }
            }
        }
        line++;
    }
}

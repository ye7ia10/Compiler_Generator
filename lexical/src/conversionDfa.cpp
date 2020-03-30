#include "../include/conversionDfa.h"
#include <algorithm>
#include "../include/stringParsing.h"

conversionDfa::conversionDfa(int numStates, vector<string> initialStates, vector<string> finalStates,
                             vector<State> nfaTransitions, vector<string> inputsTags, vector<string> statesTags
                             , vector<string>finalStatesNameOrdered, unordered_map<int, int>finalStatesPriority)
{
    this->numStates = numStates;
    this->initialStates = initialStates;
    this->finalStates = finalStates;
    this->nfaTransitions = nfaTransitions;
    this->inputsTags = inputsTags;
    this->statesTags = statesTags;
    this->finalStatesNameOrdered = finalStatesNameOrdered;
    this->finalStatesPriority = finalStatesPriority;
}

string conversionDfa::getEClosure(string states)
{
    stack<string> st;
    set<string> closure;

    vector<string> splitedStates = splitByChar(states, ',');
    for (string state : splitedStates) {
        trim(state);
        if (state.size() == 0) continue;
        st.push(state);
        closure.insert(state);
    }
    while (!st.empty())
    {
        string currentState = st.top();
        st.pop();

        int curstate = stoi(currentState);

        if (nfaTransitions[curstate].getTranstion(' ').size()  > 0) {
            string next = nfaTransitions[curstate].getTranstion(' ');
            splitedStates = splitByChar(next, ',');
            for (string state : splitedStates) {
                if (closure.find(state) == closure.end())
                {
                    closure.insert(state);
                    st.push(state);
                }
            }
        }

    }
    string res = "";
    for (auto it : closure) {
        res += (it) + ",";
    }
    return res.substr(0, res.size() - 1);

}

string conversionDfa::applyInput(string currState, string currInput)
{


    string resultState = "";

    vector<string>splitedStates = splitByChar(currState, ',');
    for(auto s : splitedStates)
    {
        int state = stoi(s);
        string transitions = nfaTransitions[state].getTranstion(currInput[0]);
        if (transitions.size() == 0) continue; /* no transtions */

        if (resultState.size() == 0) {
            resultState = transitions;
        } else {
            resultState += ',' + transitions;
        }
    }
    return resultState;
}

vector<string> conversionDfa::defaultRow()
{
    vector<string> row;

    for (int i = 0; i < inputsTags.size() - 1; i++)
    {
        row.push_back("-");
    }

    return row;
}

void conversionDfa::convertToDfa()
{
    /* as the initial state is 0 from nfa */
    string initial = "0";
    stack<string> st;

    string initialStateDfa = getEClosure(initial);
    dfaStatesMap[initialStateDfa] = 0;
    dfaStates.push_back(initialStateDfa);
    st.push(initialStateDfa);
    int dfaStateCounter = 1;
    vector<string> row = defaultRow();

    dfaTransitions.push_back(row);

    while (!st.empty())
    {
        string currState = st.top();
        st.pop();

        for (int i = 0; i < inputsTags.size() - 1; i++)
        {
            string nextState = applyInput(currState, inputsTags[i]);
            nextState = getEClosure(nextState);

            if (dfaStatesMap.find(nextState) == dfaStatesMap.end() && nextState != "")
            {
                dfaStatesMap[nextState] = dfaStateCounter;
                dfaStates.push_back(nextState);
                dfaStateCounter++;
                dfaTransitions.push_back(row);

                st.push(nextState);
            }

            int state = dfaStatesMap[currState];

            if (nextState != "")
            {
                dfaTransitions[state][i] = nextState;
            }
            else
            {
                dfaTransitions[state][i] = "-";
            }
        }
    }
}

void conversionDfa::Minimize()
{

    SetUpVars(); //for now it will be useless as it will be marked will making the conversion
    map <string, vector<string> > finalStatesGroups;
    vector<string> vec[2];

    finalStatesGroups = groupingFinalStates();

    for (int i = 0; i < n; i++)
    {
        vec[finalStatesMark[i]].push_back(dfaStates[i]);
    }

    if (vec[1].size() > 0)
    {
        pi[0].push_back(vec[1]);
    }

    std::map<std::string, vector<string> >::iterator it = finalStatesGroups.begin();
    while (it != finalStatesGroups.end())
    {
        vector<string> groupStates = it -> second;
        if (groupStates.size() > 0)
        {
            pi[0].push_back(groupStates);
        }
        it++;
    }
    pi[1] = pi[0];
    while (true)
    {
        pi[0] = pi[1];



        for (int i = 0; i < pi[1].size(); i++)
        {
            for (int j = 1; j < pi[1][i].size(); j++)
            {

                bool inPlace = false;
                for (int c = 0; c < j; c++)
                {

                    bool with = checkMatching(pi[1][i][j], pi[1][i][c]);

                    if (with)
                    {
                        //remain in same place
                        inPlace = true;
                        break;
                    }
                }
                if (inPlace)
                {
                    continue;
                }

                for (int x = pi[0].size(); x < pi[1].size(); x++)
                {
                    for (int k = 0; k < pi[1][x].size(); k++)
                    {

                        bool with = checkMatching(pi[1][i][j], pi[1][x][k]);
                        if (with)
                        {
                            pi[1][x].push_back(pi[1][i][j]);
                            pi[1][i].erase(pi[1][i].begin() + j);
                            j--;
                            inPlace = 1;
                            break;
                        }

                    }
                    if (inPlace)
                    {
                        break;
                    }
                }
                if (inPlace)
                {
                    continue;
                }
                vector<string> newPi;
                newPi.push_back(pi[1][i][j]);
                pi[1].push_back(newPi);
                pi[1][i].erase(pi[1][i].begin() + j);
                j--;
            }
        }
        bool done = CheckIfDone();
        if (done)
        {
            break;
        }
    }


}

void conversionDfa::SetUpVars()
{
    n = dfaTransitions.size();
    m = inputsTags.size() - 1;

    for (int i = 0; i < dfaStates.size(); i++)
    {
        for (int j = 0; j < finalStates.size(); j++)
        {
            vector<string> temp = splitByChar(dfaStates[i], ',');
            if (find(temp.begin(), temp.end(), finalStates[j]) != temp.end())
            {
                finalStatesMark.push_back(false);
                dfaStatesFinal.insert(pair<string, int>(dfaStates[i], -1));
                break;
            }
            if (j == finalStates.size() - 1)
            {

                finalStatesMark.push_back(true);
            }
        }
    }
}

bool conversionDfa::checkMatching(string state1, string state2)
{
    vector<string> transOne;
    vector<string> transTwo;
    for (int i = 0; i < dfaStates.size(); i++)
    {
        if (dfaStates[i] == state1)
        {
            transOne = dfaTransitions[i];
        }
        if (dfaStates[i] == state2)
        {
            transTwo = dfaTransitions[i];
        }
    }
    for (int i = 0; i < m; i++)
    {
        if (getPartitionIdx(transOne[i]) != getPartitionIdx(transTwo[i]))
        {
            return false;
        }
    }
    return true;
}

int conversionDfa::getPartitionIdx(string state)
{
    int idx = -1;
    for (int i = 0; i < pi[0].size(); i++)
    {
        for (int j = 0; j < pi[0][i].size(); j++)
        {
            if (pi[0][i][j] == state)
            {
                idx = i;
                break;
            }
        }
    }
    return idx;
}

bool conversionDfa::sameKind(string state1, string state2)
{
    bool st1K;
    bool st2K;
    int idx1;
    int idx2;
    for (int i = 0; i < dfaStates.size(); i++)
    {
        if (dfaStates[i] == state1)
        {
            idx1 = i;
        }
        if (dfaStates[i] == state2)
        {
            idx2 = i;
        }
    }
    st1K = finalStatesMark[idx1];
    st2K = finalStatesMark[idx2];
    if (st1K == st2K)
    {
        return true;
    }
    return false;
}

bool conversionDfa::CheckIfDone()
{
    for (int i = 0; i < pi[0].size(); i++)
    {
        sort(pi[0][i].begin(), pi[0][i].end());
    }
    sort(pi[0].begin(), pi[0].end());
    for (int i = 0; i < pi[1].size(); i++)
    {
        sort(pi[1][i].begin(), pi[1][i].end());
    }
    sort(pi[1].begin(), pi[1].end());
    if (pi[0] == pi[1])
    {
        return true;
    }
    return false;
}

void conversionDfa::getTheMinimizedDFATable()
{
    vector<string> representatives;
    for (int i = 0; i < pi[0].size(); i++)
    {
        representatives.push_back(pi[0][i][0]);
    }
    for (int i = 0; i < dfaTransitions.size(); i++)
    {
        if (find(representatives.begin(), representatives.end(), dfaStates[i]) != representatives.end())
        {
            /*** found ***/
            for (int j = 0; j < dfaTransitions[i].size(); j++)
            {
                if (dfaTransitions[i][j] != "-")
                {
                    dfaTransitions[i][j] = representatives[getPartitionIdx(dfaTransitions[i][j])];
                }
            }
        }
        else
        {
            /*** not found delete from and decrease i ***/
            dfaTransitions.erase(dfaTransitions.begin() + i);
            dfaStates.erase(dfaStates.begin() + i);
            finalStatesMark.erase(finalStatesMark.begin() + i);
            i--;
        }
    }
}

vector<vector<string>> conversionDfa::getTransitionTable()
{
    convertToDfa();

    Minimize();
    getTheMinimizedDFATable();

    return changeConventionTransitions();
}

vector<string> conversionDfa::getDfaStates()
{
    return changeConventionStates();
}

vector<vector<string>> conversionDfa::changeConventionTransitions()
{
    vector<vector<string>> oldTransitions = dfaTransitions;
    vector<vector<string>> newTransitions;
    vector<string> row = defaultRow();

    for (int i = 0; i < dfaStates.size(); i++)
    {
        newTransitions.push_back(row);
        mapStates.insert(pair<string, string>(dfaStates[i], to_string(i)));

        if (dfaStatesFinal.find(dfaStates[i]) != dfaStatesFinal.end())
        {
            dfaStatesFinalNew.insert(pair<string, int>(to_string(i), dfaStatesFinal[dfaStates[i]]));
        }
    }

    for (int i = 0; i < oldTransitions.size(); i++)
    {
        for (int j = 0; j < oldTransitions[i].size(); j++)
        {
            if (oldTransitions[i][j] != "-")
            {
                newTransitions[i][j] = mapStates[oldTransitions[i][j]];
            }
        }
    }

    return newTransitions;
}

vector<string> conversionDfa::changeConventionStates()
{
    vector<string> oldStates = dfaStates;
    vector<string> newStates;

    for (int i = 0; i < oldStates.size(); i++)
    {
        newStates.push_back(mapStates[oldStates[i]]);
    }
    return newStates;
}

map<string, vector<string>> conversionDfa::groupingFinalStates()
{

    vector<string> groupStates;
    map<string, vector<string>> groups;
    map<string, bool> dfaStatesCopy;

    for (int i = 0; i < dfaStates.size(); i++)
    {
        dfaStatesCopy.insert(pair<string, bool>(dfaStates[i], false));
    }

    for (int i = 0; i < finalStatesNameOrdered.size(); i++)
    {
        groupStates.clear();
        for (int j = 0; j < dfaStates.size(); j++)
        {
            vector<string> temp = splitByChar(dfaStates[j], ',');
            if ( (find(temp.begin(), temp.end(), finalStatesNameOrdered[i]) != temp.end())&& !dfaStatesCopy[dfaStates[j]])
            {
                //  Printing for testing
                //cout << "added " << dfaStates[j] << endl;
                groupStates.push_back(dfaStates[j]);
                dfaStatesFinal[dfaStates[j]] = finalStatesPriority[stoi(finalStatesNameOrdered[i])];
                dfaStatesCopy[dfaStates[j]] = true;

            }
            if (j == dfaStates.size() - 1)
            {
                groups.insert(pair<string, vector<string>>(to_string(finalStatesPriority[stoi(finalStatesNameOrdered[i])]), groupStates));
            }
        }
    }

    return groups;
}

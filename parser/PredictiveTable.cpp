#include "PredictiveTable.h"
#include "Rules.h"
#include "Rule.h"
#include "RuleComponent.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <bits/stdc++.h>
#include <string>
const string epsilon = "\'\\L\'";
const string synch = "Synch";

PredictiveTable::PredictiveTable(Rules* rules)
{
    buildTable(rules);
}

PredictiveTable::~PredictiveTable()
{
    //dtor
}
void PredictiveTable::buildTable(Rules* rules)
{
    cout << "build rules" << endl;
    map <string, Rule*>::iterator it;
    map <string, Rule*> mp = rules->getRules();
    it = mp.begin();
    nonTerminalNumber = 0;
    terminalNumber = 0;
    for(string terminalName : rules->getTerminalNames())
    {

        if(terminalName != epsilon)
        {
            cout << terminalName << "   ";
            terminals[terminalName] = terminalNumber;
            terminalNumber++;
        }
    }
    cout << endl;

    for(string nonTerminalName : rules->getNonTerminalNames())
    {
        cout << nonTerminalName<<  "   ";
        nonTerminals[nonTerminalName] = nonTerminalNumber;
        nonTerminalNumber++;
        if(auto i=mp.find(nonTerminalName+"\`") != mp.end())
        {
            cout << nonTerminalName+"\`" <<  "   ";
            nonTerminals[nonTerminalName+"\`"] = nonTerminalNumber;
            nonTerminalNumber++;
        }
    }
    cout << endl;
    table.resize(nonTerminalNumber);
    for(int i=0 ; i<nonTerminalNumber ; i++)
    {
        table[i].resize(terminalNumber);
    }
    for (int i=0 ; i<nonTerminalNumber ; i++)
    {
        for(int j=0 ; j<terminalNumber ; j++)
        {
            table[i][j] == "";
        }
    }
    int terminalN;
    int nonTerminalN;
    bool hasEpsilon = false;
    while (it != mp.end())
    {
        Rule* rule = it->second;
        string ruleName = rule->getName();
        cout << "rule name " << ruleName << endl;
        vector<Production*> productions = rule->getProductions();

        for (int i=0 ; i<productions.size() ; i++)
        {
            Production* production = productions[i];
            for (RuleComponent* ruleCompForFirst : production->getFirst())
            {
                if(ruleCompForFirst->getName() != epsilon)
                {
                    hasEpsilon = false;
                    terminalN = terminals[ruleCompForFirst->getName()];
                    nonTerminalN = nonTerminals[ruleName];
                    if(table[nonTerminalN][terminalN] == "")
                    {
                        cout << "Putting for first in:" <<nonTerminalN<< " "<< terminalN << "terminal: "<< ruleCompForFirst->getName()<<
                             "production: "<<production->getName()<<endl;
                        table[nonTerminalN][terminalN] = production->getName();
                    }
                    /* else
                     {
                         //error for ambiguity
                     }*/
                }
                else
                {
                    cout <<"epsilon " << ruleCompForFirst->getName()<<endl;
                    hasEpsilon = true;
                    for (RuleComponent* ruleCompForFollow : rule->getFollowVector())
                    {
                        cout <<"follow " << ruleCompForFollow->getName()<<endl;
                        terminalN = terminals[ruleCompForFollow->getName()];
                        nonTerminalN = nonTerminals[ruleName];
                        if(table[nonTerminalN][terminalN] == "")
                        {
                            cout << "Putting for follow in:" <<nonTerminalN << " "<< terminalN << "terminal: "<< ruleCompForFollow->getName()<<
                                 "production: "<<production->getName()<<endl;
                            table[nonTerminalN][terminalN] = production->getName();
                        }

                    }
                }
            }

        }
        if(!hasEpsilon)
        {
            for (RuleComponent* ruleCompForSynch : rule->getFollowVector())
            {
                terminalN = terminals[ruleCompForSynch->getName()];
                nonTerminalN = nonTerminals[ruleName];
                if(table[nonTerminalN][terminalN] == "")
                {
                    table[nonTerminalN][terminalN] = synch;
                    cout << "Putting for synch in:" <<nonTerminalN<< " "<< terminalN << "terminal: "<< ruleCompForSynch->getName()<<endl;
                }
            }
            hasEpsilon = false;
        }
        cout<<"************************************************"<<endl;
        it++;
    }
    for (int i=0 ; i<nonTerminalNumber ; i++)
    {
        for(int j=0 ; j<terminalNumber ; j++)
        {
            if (table[i][j] == "")
                table[i][j] = "error";
        }
    }
    unordered_map<string,int>:: iterator p;
    cout << " \t";
    p = terminals.begin();
    while(p != terminals.end())
    {
        cout << p->first << "\t";
        p++;
    }
    cout << endl;
    cout << terminalNumber << endl;
    p = nonTerminals.begin();
    while (p != nonTerminals.end())
    {
        cout << p->first << "\t" ;
        for (int j=0 ; j<terminalNumber ; j++)
        {
            cout << table[p->second][j] << "\t";
        }
        cout << endl;
        p++;
    }
}


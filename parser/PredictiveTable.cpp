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

    it = rules->getRules().begin();
    nonTerminalNumber = 0;
    terminalNumber = 0;
    for(string terminalName : rules->getTerminalNames())
    {
        cout << terminalName << "   ";
        terminals[terminalName] = terminalNumber;
        terminalNumber++;
    }
    cout << endl;
    for(string nonTerminalName : rules->getNonTerminalNames())
    {
        cout << nonTerminalName<<  "   ";
        nonTerminals[nonTerminalName] = nonTerminalNumber;
        nonTerminalNumber++;
    }
    cout << endl;
    table.resize(nonTerminalNumber+1);
    for(int i=0 ; i<=nonTerminalNumber ; i++)
    {
        table[i].resize(terminalNumber+1);
    }
    int terminalN;
    int nonTerminalN;
    bool hasEpsilon = false;
    cout << "it " << it->first << endl;
    while (it != rules->getRules().end())
    {
        cout << "rules" << endl;
        Rule* rule = it->second;
        string ruleName = rule->getName();
        cout << "rule name" << ruleName << endl;
        vector<Production*> productions = rule->getProductions();

        for (int i=0 ; i<productions.size() ; i++)
        {cout << "production" << endl;
            Production* production = productions[i];
            for (RuleComponent* ruleCompForFirst : production->getFirst())
            {
                if(ruleCompForFirst->getName() != epsilon)
                {
                    terminalN = terminals[ruleCompForFirst->getName()];
                    nonTerminalN = nonTerminals[ruleName];
                    if(table[nonTerminalN][terminalN] == "" || (table[nonTerminalN][terminalN] == production->getName()))
                    {
                        table[nonTerminalN][terminalN] = production->getName();
                    }
                    else
                    {
                        //error for ambiguity
                    }
                }
                else
                {
                    hasEpsilon = true;
                    for (RuleComponent* ruleCompForFollow : rule->getFollowVector())
                    {
                        terminalN = terminals[ruleCompForFollow->getName()];
                        nonTerminalN = nonTerminals[ruleName];
                        if(table[nonTerminalN][terminalN] == "")
                        {
                            table[nonTerminalN][terminalN] = production->getName();
                        }
                        else
                        {
                            //error for ambiguity
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
                    if(table[nonTerminalN][terminalN] == ""){
                        table[nonTerminalN][terminalN] = synch;
                    }
                }
                hasEpsilon = false;
            }
        }
    it++;
    }
    for (int i=0 ; i<=nonTerminalNumber ; i++){
        for(int j=0 ; j<=terminalNumber ; j++){
            if (table[i][j] == "")
                table[i][j] == "error";
        }
    }
    unordered_map<string,int>:: iterator p;
    for (p = terminals.begin() ; p!=terminals.end() ; p++){
        cout << p->first << "     ";
    }
    cout << endl;
    p = nonTerminals.begin();
    for (int i=0 ; i<=nonTerminalNumber ; i++){
        for (int j=0 ; j<=terminalNumber ; j++){
            cout << p->first << "     " << table[i][j] << endl;
        }
        p++;
    }
}

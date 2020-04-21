#include "PredictiveTable.h"
#include "Rules.h"
#include "Rule.h"
#include "RuleComponent.h"
#include <iostream>
#include <algorithm>
#include <vector>
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
    map <string, Rule*>::iterator it;

    it = rules->getRules().begin();
    nonTerminalNumber = 0;
    terminalNumber = 0;
    for(string terminalName : rules->getTerminalNames())
    {
        terminals[terminalName] = terminalNumber;
        terminalNumber++;
    }
    for(string nonTerminalName : rules->getNonTerminalNames())
    {
        nonTerminals[nonTerminalName] = nonTerminalNumber;
        nonTerminalNumber++;
    }
    table.resize(nonTerminalNumber+1);
    for(int i=0 ; i<=nonTerminalNumber ; i++)
    {
        table[i].resize(terminalNumber+1);
    }
    int terminalN;
    int nonTerminalN;
    bool hasEpsilon = false;
    while (it != rules->getRules().end())
    {
        Rule* rule = it->second;
        string ruleName = rule->getName();
        for (Production* production : rule->getProductions())
        {
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

    }
    for (int i=0 ; i<=nonTerminalNumber ; i++){
        for(int j=0 ; j<=terminalNumber ; j++){
            if (table[i][j] == "")
                table[i][j] == "error";
        }
    }
}

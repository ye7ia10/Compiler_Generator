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
const string assignSymbol = "\'=\'";
const string assignText = "\'assign\'";

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
    //cout << "build rules" << endl;
    map <string, Rule*>::iterator it;
    map <string, Rule*> mp = rules->getRules();
    it = mp.begin();
    terminals.clear();
    nonTerminals.clear();
    nonTerminalNumber = 0;
    terminalNumber = 0;
    while(it != mp.end())
    {
        Rule* rule = it->second;
        string nonTerminalName = rule->getName();
        if(nonTerminals.find(nonTerminalName) == nonTerminals.end())
        {
            nonTerminals[nonTerminalName] = nonTerminalNumber;
            nonTerminalNumber++;
            if(mp.find(nonTerminalName+"\`") != mp.end())
            {
                nonTerminals[nonTerminalName+"\`"] = nonTerminalNumber;
                nonTerminalNumber++;
            }
        }
        for (Production* production : rule->getProductions())
        {
            vector<RuleComponent*> ruleComponent = production->getElements();
            for(RuleComponent* comp : ruleComponent)
            {
                if(comp->isTerminal())
                {
                    string terminalName = comp->getName();
                    if(terminalName!=epsilon && terminals.find(terminalName)==terminals.end())
                    {
                        if (terminalName == assignSymbol)
                        {
                            terminalName = assignText;
                        }
                        terminals[terminalName] = terminalNumber;
                        terminalNumber++;

                    }
                }
            }
        }
        it++;
    }

    it = mp.begin();
    terminals["$"] = terminalNumber;
    terminalNumber++;

    /*for (int k=0 ; k<terminalNumber ; k++)
    {
        for(auto i=terminals.begin() ; i!=terminals.end() ; i++)
        {
            if(i->second == k)
                cout << i->first << "   " << i->second << endl;
        }
    }*/


    //cout << endl;

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
    //cout << "terminal size: " << terminalNumber<< endl;

    int terminalN;
    int nonTerminalN;
    bool hasEpsilon = false;
    while (it != mp.end())
    {
        Rule* rule = it->second;
        string ruleName = rule->getName();
        //cout << "rule name " << ruleName << endl;
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
                    if(ruleCompForFirst->getName() == assignSymbol)
                        terminalN = terminals[assignText];
                    nonTerminalN = nonTerminals[ruleName];
                    if(table[nonTerminalN][terminalN] == "")
                    {
                       // cout << "Putting for first in:" <<nonTerminalN<< " "<< terminalN << "terminal: "<< ruleCompForFirst->getName()<<
                             //"production: "<<production->getName()<<endl;
                        table[nonTerminalN][terminalN] = production->getName();
                    }
                }
                else
                {
                    //cout <<"epsilon " << ruleCompForFirst->getName()<<endl;
                    hasEpsilon = true;
                    for (RuleComponent* ruleCompForFollow : rule->getFollowVector())
                    {
                        //cout <<"follow " << ruleCompForFollow->getName()<<endl;
                        terminalN = terminals[ruleCompForFollow->getName()];
                        if(ruleCompForFollow->getName() == assignSymbol)
                            terminalN = terminals[assignText];
                        nonTerminalN = nonTerminals[ruleName];
                        if(table[nonTerminalN][terminalN] == "")
                        {
                            //cout << "Putting for follow in:" <<nonTerminalN << " "<< terminalN << "terminal: "<< ruleCompForFollow->getName()<<
                                 //"production: "<<production->getName()<<endl;
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
                    //cout << "Putting for synch in:" <<nonTerminalN<< " "<< terminalN << "terminal: "<< ruleCompForSynch->getName()<<endl;
                }
            }
            hasEpsilon = false;
        }
        //cout<<"************************************************"<<endl;
        it++;
    }
    for (int i=0 ; i<nonTerminalNumber ; i++)
    {
        for(int j=0 ; j<terminalNumber ; j++)
        {
            if (table[i][j] == "")
                table[i][j] = "Error";
        }
    }
    predectiveTableFilePrint();
}
void PredictiveTable::predectiveTableFilePrint()
{
    ofstream fout;
    fout.open("PredectiveTable.txt");
    string sep = " |";
    int noOfC = ((terminalNumber+1)*40) + (sep.size()*terminalNumber+1);
    string line = sep + string(noOfC-1,'-') + " |";
    if (fout.is_open())
    {
        fout << line << endl << sep;
        fout << left << setw(40) << ""<< sep;
        for (int i=0 ; i<terminalNumber ; i++)
        {
            for(auto it=terminals.begin() ; it!=terminals.end() ; it++)
            {
                if (it->second == i)

                    fout << left << setw(40) << it->first << sep;

            }
        }
        for(int i=0 ; i<nonTerminalNumber ; i++)
        {
            for (auto it=nonTerminals.begin() ; it!=nonTerminals.end() ; it++)
            {
                if(it->second == i)
                {
                    fout << endl << line << endl << sep;
                    fout << left << setw(40) << it->first << sep;

                    for (int j=0 ; j<terminalNumber ; j++)
                    {
                        fout << left << setw(40) << table[it->second][j] << sep;
                    }

                }
            }
        }
        fout << endl << line ;
        fout.close();
    }
}


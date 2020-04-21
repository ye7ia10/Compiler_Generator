#include "Parser.h"
#include "PredictiveTable.h"
Parser::Parser(string fileName)
{

    rules = new Rules();
    AnalyzeRules* analyze = new AnalyzeRules(fileName, rules);

    rules->removeLeftRecursion();
    rules->removeLeftFactoring();


    rules->calcFirst();
    rules->calcFollow();
    for (auto r : rules->getRules()) {
        r.second->toString();
        for (Production* p : r.second->getProductions()) {
            p->toString();
        }
        cout << "***********\n";

    }
     map <string, Rule*>::iterator it;

    it = rules->getRules().begin();
    Rule* rule = it->second;
    cout << "a7a" << rule->getName() << endl;
    PredictiveTable* table = new PredictiveTable(rules);
}

Parser::~Parser()
{
    //dtor
}

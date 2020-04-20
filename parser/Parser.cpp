#include "Parser.h"

Parser::Parser(string fileName)
{

    rules = new Rules();
    AnalyzeRules* analyze = new AnalyzeRules(fileName, rules);

    rules->removeLeftRecursion();



    rules->calcFirst();
    for (auto r : rules->getRules()) {
        r.second->toString();
        for (Production* p : r.second->getProductions()) {
            p->toString();
        }
        cout << "***********\n";

    }
}

Parser::~Parser()
{
    //dtor
}

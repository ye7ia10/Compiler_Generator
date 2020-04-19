#ifndef RULE_H
#define RULE_H
#include "../lexical/include/stringParsing.h"
#include "Production.h"
class Rule
{
    public:
        Rule(string nonTerminalName, vector<string> rHS);
        Rule(string nonTerminalName, string rHS);
        virtual ~Rule();
        vector<string> getProductionsString();
        string getName();

    protected:

    private:
    string name;
    vector<Production*> productions;
    vector<RuleComponent*> first;
    vector<RuleComponent*> follow;
    vector<string> productionsString;
    void buildRule(string rHS);

};

#endif // RULE_H

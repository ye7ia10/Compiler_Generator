#ifndef RULE_H
#define RULE_H
#include "Production.h"
class Rule
{
    public:
        Rule(string nonTerminalName, vector<string> rHS);
        virtual ~Rule();
        vector<string> getProductionsString();

    protected:

    private:
    vector<Production*> productions;
    vector<RuleComponent*> first;
    vector<RuleComponent*> follow;
    vector<string> productionsString;

};

#endif // RULE_H

#ifndef RULE_H
#define RULE_H
#include "Production.h"
class Rule
{
    public:
        Rule();
        virtual ~Rule();

    protected:

    private:
    vector<Production*> productions;
    vector<RuleComponent*> first;
    vector<RuleComponent*> follow;
};

#endif // RULE_H

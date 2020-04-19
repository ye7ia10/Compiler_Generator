#ifndef PRODUCTION_H
#define PRODUCTION_H
#include "../lexical/include/stringParsing.h"
#include "RuleComponent.h"

class Production
{
    public:
        Production(string s);
        virtual ~Production();
        vector<RuleComponent*> elements;

    protected:

        vector<RuleComponent*> first;
        void buildProduction(string s);
    private:
};

#endif // PRODUCTION_H

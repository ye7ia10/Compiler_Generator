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
        vector<RuleComponent*> getElements();
        RuleComponent* getRlueComponent(int i);
        void addFirst(vector<RuleComponent*> first);
        void toString();
        vector<RuleComponent*> getFirst();
        string getName();
    protected:
        string name;
        vector<RuleComponent*> first;
        void buildProduction(string s);
        void removeDubLicates(vector<RuleComponent*>& v);
    private:
};

#endif // PRODUCTION_H

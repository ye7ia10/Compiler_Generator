#ifndef PRODUCTION_H
#define PRODUCTION_H
#include "RuleComponent.h"

class Production
{
    public:
        Production();
        virtual ~Production();

    protected:
        vector<RuleComponent*> elements;
        vector<RuleComponent*> first;
    private:
};

#endif // PRODUCTION_H

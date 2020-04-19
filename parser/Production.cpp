#include "Production.h"

Production::Production(string s)
{
    buildProduction(s);
}

Production::~Production()
{
    //dtor
}


void Production::buildProduction(string s)
{
    vector<string> splited = splitByChar(s, ' ');
    for (string elm : splited)
    {
        if (elm != " ")
        {
            RuleComponent* component = new RuleComponent(elm);
            this->elements.push_back(component);
        }

    }
}

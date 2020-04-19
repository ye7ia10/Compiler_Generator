#include "RuleComponent.h"

RuleComponent::RuleComponent(string s)
{
    buildComponent(s);
}

RuleComponent::~RuleComponent()
{
    //dtor
}

void RuleComponent::buildComponent(string s)
{
    if (s.size() >= 2 && s[0] == '\'' && s[s.size() - 1] == '\'')
    {
        terminal = true;
    }
    else
    {
        terminal = false;
    }
    this-> name = s;
}

string RuleComponent::getName()
{
    return this->name;
}

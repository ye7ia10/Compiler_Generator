#ifndef RULECOMPONENT_H
#define RULECOMPONENT_H
#include<bits/stdc++.h>
using namespace std;

class RuleComponent
{
    public:
        RuleComponent();
        virtual ~RuleComponent();
        string getName();
        bool isTerminal();
    protected:

    private:
    string name;
    bool inTerminal;

};

#endif // RULECOMPONENT_H

#ifndef RULECOMPONENT_H
#define RULECOMPONENT_H
#include<bits/stdc++.h>
using namespace std;

class RuleComponent
{
    public:
        RuleComponent(string s);
        virtual ~RuleComponent();
        string getName();
        bool isTerminal();
    protected:

    private:
    string name;
    bool terminal;
    void buildComponent(string s);

};

#endif // RULECOMPONENT_H

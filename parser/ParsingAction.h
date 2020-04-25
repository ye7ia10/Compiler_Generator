#ifndef PARSINGACTION_H
#define PARSINGACTION_H
#include <bits/stdc++.h>
using namespace std;

class ParsingAction
{
public:
    ParsingAction(string rule, string production);
    virtual ~ParsingAction();
    string getRule();
    string getProduction();


protected:

private:

    string rule;
    string production;

};

#endif // PARSINGACTION_H

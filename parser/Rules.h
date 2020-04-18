#ifndef RULES_H
#define RULES_H
#include "Rule.h"
class Rules
{
    public:
        Rules();
        virtual ~Rules();
        void addRule(Rule* rule);
        int numberOfrules();
        Rule* getRule(int i);
        void calcFirst();
        void calcFollow();
        void removeLeftFactoring();
        void removeLeftRecursion();
        bool hasLeftRecusrion(string CurNonTerm, vector<string> currentRuleProd);
        vector<string> replaceNons (vector<string> curProd, string tempName , vector<string> tempProd);
        void EliminateLRCurrenRule(vector<string>currentRuleProd, string curNon);

    protected:

    private:
    unordered_map <string, Rule*> rules;
    vector<string> NonTerminalsNames;
};

#endif // RULES_H

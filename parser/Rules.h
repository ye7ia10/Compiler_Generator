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
        map <string, Rule*>  getRules();

    protected:

    private:
    map <string, Rule*> rules;
    vector<string> NonTerminalsNames;
    int findMinLength(vector<string> arr, int startIndex, int endIndex);
    bool allContainsPrefix(vector<string> arr, string str,
                       int start, int end, int startIndex, int endIndex);
    string commonPrefix(vector<string> arr, int startIndex, int endIndex);
    void modifyRule(vector<string> &modifiedRule, string prefix, Rule* currentRule);
    void addRuleLeftFactoring(vector<string> &modifiedRule, string prefix, string nonTerminal, Rule* currentRule);
    void partialModifiedRule(vector<string> &modifiedRule, string prefix, string nonTerminal, Rule* currentRule);
    bool mycomp(string a, string b);
    vector<RuleComponent*>calcFirstByRec(Rule* r);

};

#endif // RULES_H

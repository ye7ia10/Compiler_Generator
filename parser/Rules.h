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
    vector<string> replaceNons (vector<string> curProd, string tempName, vector<string> tempProd);
    void EliminateLRCurrenRule(vector<string>currentRuleProd, string curNon, map<string, Rule*> &newMap);
    map <string, Rule*>  getRules();
    void getFollow( map<string, Rule*>::iterator it, string nonTe, set<string>& vis);
    void getFollowByFirst(RuleComponent* nextComponent, int idxComponent,
                          map<string, Rule*>::iterator it, map<string, Rule*>::iterator itMap,
                          vector<RuleComponent*> components, set<string>& vis);
    void addDollarToFirstRule();
    void setFirstRule(string rule);
    string getFirstRule();
    vector<string> getNonTerminalNames();


protected:

private:
    map <string, Rule*> rules;
    vector<string> NonTerminalsNames;

    void modifyRule(vector<string> &modifiedRule, string prefix, Rule* currentRule);
    void addRuleLeftFactoring(vector<string> &modifiedRule, string prefix, string nonTerminal, Rule* currentRule);
    bool mycomp(string a, string b);
    vector<RuleComponent*>calcFirstByRec(Rule* r);
    string firstRuleString;
    map<string, bool> visitedFollow;
    void otherOccurences (vector<vector<string>> &splited, string &prefix, int searchStart, int prefixNo, int compNo);
    int getLastindex (vector<string> &s, int start, int arrSize);

};

#endif // RULES_H

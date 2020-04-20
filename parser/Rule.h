#ifndef RULE_H
#define RULE_H
#include "../lexical/include/stringParsing.h"
#include "Production.h"
class Rule
{
    public:
        Rule(string nonTerminalName, vector<string> rHS);
        Rule(string nonTerminalName, string rHS);
        virtual ~Rule();
        vector<string> getProductionsString();
        string getName();
        void setName(string newName);
        void setProductionString(vector<string> productionStringNew);
        void setProductions(vector<Production*> productions);
        vector<Production*> productions;
        vector<RuleComponent*> getFirst();
        void putFirst(vector<RuleComponent*> first);
        vector<Production*>getProductions();
        void toString();
        vector<RuleComponent*> getFollowVector();
        void addFollow(RuleComponent* c);
    protected:

    private:
    string name;

    vector<RuleComponent*> first;
    vector<RuleComponent*> follow;
    vector<string> productionsString;
    void buildRule(vector<string> rHS);
    void removeDubLicates(vector<RuleComponent*>& v);

};

#endif // RULE_H

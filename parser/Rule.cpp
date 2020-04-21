#include "Rule.h"

Rule::Rule(string nonTerminalName, vector<string> rHS)
{
    name = nonTerminalName;

    buildRule(rHS);
}
Rule::Rule(string nonTerminalName, string rHS)
{
    name = nonTerminalName;
    vector<string>productions = splitByChar(rHS, '|');
    buildRule(productions);
}
Rule::~Rule()
{
    //dtor
}

vector<string>  Rule::getProductionsString()
{
    return this -> productionsString;
}

void Rule::buildRule(vector<string>productions)
{


    this->productionsString = productions;
    for (string s : productions)
    {
        trim(s);
        Production* production = new Production(s);
        this->productions.push_back(production);
    }

}

string Rule::getName()
{
    return name;
}

void Rule::setProductionString(vector<string> productionStringNew)
{
    this->productionsString = productionStringNew;
}

void Rule::setName(string newName)
{
    this->name = newName;
}

void Rule::setProductions(vector<Production*> productions)
{
    this->productions = productions;
}
vector<RuleComponent*> Rule::getFirst() {
    return first;
}
void Rule::putFirst(vector<RuleComponent*> first) {
    removeDubLicates(first);
    this->first = first;
}
vector<Production*> Rule::getProductions(){
    return this->productions;
}
void Rule::toString() {
    cout << "Name: " << name << endl;
    cout << "first: ";
    for (RuleComponent* c: first) {
        cout << c->getName() << " ";
    }
    cout << endl;
    cout << "Follow: ";
    for (RuleComponent* c: follow) {
        cout << c->getName() << " ";
    }
    cout << endl;

}
void Rule::removeDubLicates(vector<RuleComponent*>& v) {
    vector<RuleComponent*> c;
    for (int i = 0; i < v.size(); i++) {
        bool found = false;
        for (int j = 0; j < c.size(); j++) {
            if(v[i]->getName() == c[j]->getName()) {
                found = true;
                break;
            }
        }
        if (!found) {
            c.push_back(v[i]);
        }
    }
    v = c;
}
vector<RuleComponent*> Rule::getFollowVector (){
    return this -> follow;
}

void Rule::addFollow(RuleComponent* c) {
    bool exist = false;
    for (int i = 0; i < follow.size() ; i++){
        if (follow[i] -> getName() == c->getName()){
            exist = true;
        }
    }

    if (!exist){
         follow.push_back(c);
    }

}
void Rule::setFirstRule(bool firstRule) {
    this->firstRule = firstRule;
}

bool Rule::isFirstRule(){
    return firstRule;
}

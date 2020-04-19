#include "Rule.h"

Rule::Rule(string nonTerminalName, vector<string> rHS)
{

}
Rule::Rule(string nonTerminalName, string rHS)
{
    name = nonTerminalName;
    buildRule(rHS);
}
Rule::~Rule()
{
    //dtor
}

vector<string>  Rule::getProductionsString(){
    return this -> productionsString;
}

void Rule::buildRule(string rHS) {

    vector<string>productions = splitByChar(rHS, '|');
    this->productionsString = productions;
    for (string s : productions) {
        Production* production = new Production(s);
        this->productions.push_back(production);
    }

}

string Rule::getName() {
    return name;
}

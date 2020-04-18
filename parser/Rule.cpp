#include "Rule.h"

Rule::Rule(string nonTerminalName, vector<string> rHS)
{
    //ctor
}

Rule::~Rule()
{
    //dtor
}

vector<string>  Rule::getProductionsString(){
    return this -> productionsString;
}

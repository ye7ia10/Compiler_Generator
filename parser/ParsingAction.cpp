#include "ParsingAction.h"

ParsingAction::ParsingAction(string rule, string production)
{
    this->rule = rule;
    this->production = production;
}

ParsingAction::~ParsingAction()
{
    //dtor
}

string ParsingAction::getRule() {
    return rule;
}

string ParsingAction::getProduction() {
    return production;
}

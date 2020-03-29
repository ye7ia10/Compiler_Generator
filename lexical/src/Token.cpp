#include "../include/Token.h"

Token::Token(string name, string rule, int line, bool hasError)
{
    this->name = name;
    this->line = line;
    this->rule = rule;
    this->hasError = hasError;
}

Token::~Token()
{
    //dtor
}

string Token::getName() {
    return name;
}

string Token::getRule() {
    return rule;
}

int Token::getLine() {
    return line;
}

bool Token::isError() {
    return hasError;
}
string Token::toString()
{
    if (hasError)
    {
        return "lexical error found in line: " + to_string(line) + " : \" " + name + " \"";
    }
    else
    {
        return rule;
    }
}

void Token::addCharToName(char c)
{
    name = name + c;
}

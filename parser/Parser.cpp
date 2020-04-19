#include "Parser.h"

Parser::Parser(string fileName)
{
    rules = new Rules();
    AnalyzeRules* analyze = new AnalyzeRules(fileName, rules);
}

Parser::~Parser()
{
    //dtor
}

#ifndef PARSER_H
#define PARSER_H
#include "Rules.h"
#include "PredictiveTable.h"
#include "ParsedLine.h"
#include "AnalyzeRules.h"
#include "Token.h"
class Parser
{
    public:
        /* parse table and build rules and predictive table*/
        Parser(string fileName);
        virtual ~Parser();
        vector<ParsedLine> outputParser;


    protected:

    private:
    Rules* rules;
    PredictiveTable* table;
    vector<Token> outputLexical;
    void parsingLines(string startSymbol, PredictiveTable* table);

};

#endif // PARSER_H

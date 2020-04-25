#ifndef PARSER_H
#define PARSER_H
#include "Rules.h"
#include "PredictiveTable.h"
#include "AnalyzeRules.h"
#include "Token.h"
#include "ParsingAction.h"
class Parser
{
    public:
        /* parse table and build rules and predictive table*/
        Parser(string fileName, vector<Token> tokenVec);
        virtual ~Parser();
        vector<ParsingAction> outputParser;
        void parsingLines(string startSymbol);


    protected:

    private:
    Rules* rules;
    PredictiveTable* table;
    vector<Token> outputLexical;
    vector<string> outputStore;
    void replace(std::string& str, const std::string& from, const std::string& to);



};

#endif // PARSER_H

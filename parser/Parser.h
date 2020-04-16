#ifndef PARSER_H
#define PARSER_H
#include "Rules.h"
#include "PredictiveTable.h"
#include "AnalyzeRules.h"
class Parser
{
    public:
        /* parse table and build rules and predictive table*/
        Parser(string fileName);
        virtual ~Parser();

    protected:

    private:
    Rules* rules;
    PredictiveTable* table;

};

#endif // PARSER_H

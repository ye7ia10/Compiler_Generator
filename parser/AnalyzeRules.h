#ifndef ANALYZERULES_H
#define ANALYZERULES_H
#include "Rules.h"
#include "Rule.h"

class AnalyzeRules
{
    public:
        /* split rules and parse each rule and build rules object*/
        AnalyzeRules(string fileName, Rules* rules);
        virtual ~AnalyzeRules();

    protected:

    private:
    void buildRules(string fileName, Rules* rules);
    Rule* handleEachRule(string producationRule);

};

#endif // ANALYZERULES_H

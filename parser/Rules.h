#ifndef RULES_H
#define RULES_H
#include "Rule.h"
class Rules
{
    public:
        Rules();
        virtual ~Rules();
        void addRule(Rule* rule);
        int numberOfrules();
        Rule* getRule(int i);
        void calcFirst();
        void calcFollow();
        void removeLeftFactoring();
        void removeLeftRecursion();

    protected:

    private:
    unordered_map <string, Rule*> rules;
};

#endif // RULES_H

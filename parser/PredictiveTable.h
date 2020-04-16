#ifndef PREDICTIVETABLE_H
#define PREDICTIVETABLE_H
#include "Rules.h"

class PredictiveTable
{
    public:
        PredictiveTable(Rules* rules);
        virtual ~PredictiveTable();

    protected:

    private:
    void buildTable(Rules* rules);
    int terminalNumber;
    int nonTerminalNumber;
    map<string, int> terminals;
    map<string, int> nonTerminals;
    vector<vector<string>>table;
};

#endif // PREDICTIVETABLE_H

#ifndef PREDICTIVETABLE_H
#define PREDICTIVETABLE_H
#include "Rules.h"

class PredictiveTable
{
public:
    PredictiveTable(Rules* rules);
    virtual ~PredictiveTable();
    unordered_map<string, int> terminals;
    unordered_map<string, int> nonTerminals;
    vector<vector<string>>table;
    void predectiveTableFilePrint();

protected:

private:
    void buildTable(Rules* rules);
    int terminalNumber;
    int nonTerminalNumber;

};

#endif // PREDICTIVETABLE_H

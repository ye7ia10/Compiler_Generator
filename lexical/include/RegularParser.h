#ifndef REGULARPARSER_H
#define REGULARPARSER_H
#include"NFA.h"
#include "stringParsing.h"
using namespace std;
/* use this struct when want to return errors with NFA or check if it is operation */
struct DataEntry
{
    NFA nfa;
    bool haveError = false;
    string name = "";
    bool isOp = false;
};

class RegularParser
{
public:
    /*all rules to be parsed*/
    RegularParser(vector<string> s);
    vector<State>getTotalNFAStates();
    virtual ~RegularParser();

protected:

private:
    /* contains all nfa expression */
    vector<NFA>nfaVector;
    void parseFile(vector<string> s);
    /* contains all Definitions */
    unordered_map<string, NFA> regDefinition;

    /* periority counter to NFA states */
    int priority = 0;
    /* take string regexString represent one line of the regular file and make the regular expression to it */
    void parseLine(string regexString);


    DataEntry getNFA(string regexString);
    void addStringToNFA(string s);
    /* table contain all rule states */
    vector<State>totalNFA;
    /* split elements of regex strings */
    vector<string> split(string regexString);

    /* this function used to get the name of operation used in regular expresion */
    string getOppName(char c);
    /* check if the string s represent one of the regular operations */
    bool isOp(string s);

    /* check if dash charachter is a range operation */
    bool isDashOpp(string str1, string str2);
    /* recognize dash operation and put union rather than the dash operation in the reference vector passed in parameters*/
    void recognizeDashOpp(vector<string>& strVec);
    /*it builds the range of the dash operation.
    this function must be used after check that the two strings make a right dash operation.
    it addes the range to the reference vector using Union operation between them */
    void buildRange(NFA& nfa, string str1, string str2);

    /* make every element as NFA or call NFA of the defination used.
    also operations added with boolean osOp = true to difference it from NFA */
    vector<DataEntry> prepareNFA(vector<string> splitedString);

    /* Calculate regular expression after distingish operations from NDA expressions.
    using the same postfix algorithm but with some modifications */
    DataEntry calculateRegexWithPriority(vector<DataEntry>vec);

    /* calculate simple expression (without brackets) with proirity by closure - positive closure and so on */
    DataEntry calcExpression(vector<DataEntry>exp);
    /* add all nfa into one nfa table */
    void prepareForDfa();


};

#endif // REGULARPARSER_H

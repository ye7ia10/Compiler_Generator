#ifndef PARSEDLINE_H
#define PARSEDLINE_H
#include <bits/stdc++.h>
using namespace std;

class ParsedLine
{
public:
    ParsedLine(string inputLine, vector<string> parsed, int line, bool hasError);
    virtual ~ParsedLine();
    string getInputLine();
    vector<string> getParsedLine();
    int getLine();
    bool isError();


protected:

private:

    int line;
    vector<string> parsed;
    bool hasError;
    string inputLine;

};

#endif // PARSEDLINE_H

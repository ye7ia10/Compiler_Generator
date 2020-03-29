#ifndef TOKEN_H
#define TOKEN_H
#include <bits/stdc++.h>
using namespace std;

class Token
{
public:
    Token(string name, string rule, int line, bool hasError);
    virtual ~Token();
    string toString();
    string getName();
    string getRule();
    int getLine();
    bool isError();
    void addCharToName(char c);


protected:

private:

    int line;
    string rule;
    bool hasError;
    string name;

};

#endif // TOKEN_H

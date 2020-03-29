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
    void addCharToName(char c);
    bool hasError;
    string name;
protected:

private:

    int line;
    string rule;

};

#endif // TOKEN_H

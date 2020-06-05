%{
#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;
#define YY_DECL extern "C" int yylex()
#include "parser.tab.h"
bool lexical_error;
int cur_line = 1;

void log_error(string error);
%}

DIGIT    [0-9]
LETTER   ([a-z]|[A-Z])
DIGITS   [0-9]+

%%
[ \t] ;
[\n]                                                           { cur_line++; }

[\(\)\{\},;]                                                      { return yytext[0]; }
int                                                               { return INT; }
float                                                             { return FLOAT; }
boolean                                                           { return BOOLEAN; }
if                                                                { return IF; }
else                                                              { return ELSE; }
while                                                             { return WHILE; }
true															                                { return TRUE; }
false															                                { return FALSE; }
{LETTER}({LETTER}|{DIGIT}|"_")*                                   { yylval.id_val = strdup(yytext); return ID; }
{DIGITS}                                                          { yylval.ival = strdup(yytext); return IVAL;}
(({DIGIT}*\.{DIGITS})|({DIGITS}))(E[\+\-]?{DIGITS})?              { yylval.fval = strdup(yytext); return FVAL; }
"=="|"!="|">"|">="|"<"|"<="                                       { yylval.relop = strdup(yytext); return RELOP; }
[\=]                                                              { yylval.assign = yytext[0]; return ASSIGN; }
[\+\-]                                                            { yylval.addop = yytext[0]; return ADDOP; }
[\*/]                                                             { yylval.mulop = yytext[0]; return MULOP; }
.                                                                 { log_error("Couldn't match text{" + string(yytext) + "} at line:" + to_string(cur_line));
                                                                    log_error("Skipping match{" + string(yytext) + "}, Program might be malformed.");}
%%

void log_error(string error) {
  cout << "Error: " << error << endl;
}
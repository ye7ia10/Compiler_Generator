%{
#include <bits/stdc++.h>
using namespace std;


extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

const int MAX_LABEL_LENGTH = 10;
enum typeEnum {
    intNum, floatNum, errorType
};
map<string, pair<int,typeEnum> > symbolTable;
vector<string> code;
vector<int> codeNumbers;
int varNum = 0;
int lineNum = 0;
ofstream out("output.class");


// java byte code instructions
map<string,string> arithmeticMap = {
    {"+", "add"},
    {"-", "sub"},
    {"/", "div"},
    {"*", "mul"}
};
map<string,string> intMap = {
    {"==", "if_icmpeq"},
    {"<=", "if_icmple"},
    {">=", "if_icmpge"},
    {"!=", "if_icmpne"},
    {">",  "if_icmpgt"},
    {"<",  "if_icmplt"}
};
map<string,string> floatMap = {
    {"==", "ifeq"},
    {"<=", "ifle"},
    {">=", "ifge"},
    {"!=", "ifne"},
    {">",  "ifgt"},
    {"<",  "iflt"}
};



void addCode(string s, int line);
void generateJavaByteCode();
void backpatch(vector<int> *v, int a);
void variableInitialization(char *id_val, int type);
int getFloatOrInt(int t1, int t2);
void yyerror(const char *s);
vector<int>* combine(vector<int>*v1, vector<int>*v2);
%}

%union {
  char *ival;
  char *fval;
  char *id_val;
  struct decisionBox {
    vector<int> *falseList;
    vector<int> *next;
    vector<int> *trueList;
  } decisionBox;
  int type;
  char addop;
  char mulop;
  char assign;
  char *relop;
}
%token <ival> IVAL
%token <fval> FVAL
%token <id_val> ID
%token <addop> ADDOP
%token <mulop> MULOP
%token <relop> RELOP
%token <assign> ASSIGN

%token INT FLOAT BOOLEAN
%token IF ELSE
%token WHILE
%token TRUE FALSE

%type <type> primitive_type factor term
%type <type> simple_expression
%type <decisionBox> if statement while boolean_expression
%type <type> mark
%type <addop> sign
%start method_body
%%


method_body:
    statement_list {addCode("return", lineNum);};
statement_list:
    statement
    | statement_list statement
    ;
statement:
    declaration {$$.next = new vector<int>;}
    | if {backpatch($1.next, lineNum);}
    | while {backpatch($1.next, lineNum);}
    | assignment {$$.next = new vector<int>;}
    ;
declaration:
    primitive_type ID ';'
    {variableInitialization($2, $1);}
    ;
primitive_type:
    INT{$$ = intNum;}
    | FLOAT{$$ = floatNum;}
    ;
if:
    IF '(' boolean_expression ')' '{'
    {
        backpatch($3.trueList, lineNum);
    }
    statement
    {
        $7.next = new vector<int>;
        $7.next->push_back(code.size());
        addCode("goto ", lineNum);
        lineNum += 3;
    }
    '}' ELSE '{'
    {
       backpatch($3.falseList, lineNum);
    }
    statement '}'
    {
        $$ = $7;
        if($$.next == nullptr)
            $$.next = new vector<int>;
        if($13.next){
            $$.next = combine($$.next, $13.next);
        }
    }
    ;
while:
    WHILE '(' mark boolean_expression ')' '{'
    {
        backpatch($4.trueList, lineNum);
    }
    statement '}'
    {
        addCode("goto " + to_string($3), lineNum);
        lineNum += 3;
        $$.next = $4.falseList;
        if($$.next == nullptr)
            $$.next = new vector<int>;
    }
    ;
mark:
    {
        $$ = lineNum;
    }
    ;
assignment:
    ID ASSIGN simple_expression ';'
    {
        if(!symbolTable.count(string($1)))
            yyerror("Undeclared variable.");
        // Consider casting instead of the following.
        if(symbolTable[string($1)].second != $3)
            yyerror("Assigned a variable to an expression with different type.");
        if(symbolTable[string($1)].second == intNum){
            addCode("istore " + to_string(symbolTable[string($1)].first), lineNum);
            lineNum += 1;
        }
        else{
            addCode("fstore " + to_string(symbolTable[string($1)].first), lineNum);
            lineNum += 1;
        }
    }
    ;
boolean_expression:
    TRUE
    {
        $$.trueList = new vector<int>;
        $$.falseList = new vector<int>;
        $$.trueList->push_back(code.size());
        addCode("goto ", lineNum);
        lineNum += 3;
    }
    | FALSE
    {
        $$.trueList = new vector<int>;
        $$.falseList = new vector<int>;
        $$.falseList->push_back(code.size());
        addCode("goto ", lineNum);
        lineNum += 3;
    }
    | simple_expression RELOP simple_expression
    {
        if($1 != $3) yyerror("different types in operation error");
        $$.trueList = new vector<int>;
        $$.falseList = new vector<int>;
        if($1 == intNum){
            $$.trueList->push_back(code.size());
            addCode(intMap[string($2)] + " ", lineNum);
            lineNum += 3;
            $$.falseList->push_back(code.size());
            addCode("goto ", lineNum);
            lineNum += 3;
        }
        else{
            addCode("fcmpl", lineNum);
            lineNum += 1;
            $$.trueList->push_back(code.size());
            addCode(floatMap[string($2)] + " ", lineNum);
            lineNum += 3;
            $$.falseList->push_back(code.size());
            addCode("goto ", lineNum);
            lineNum += 3;
        }
    }
    ;
simple_expression:
    term
    | sign term
    {
        $$ = $2;
        if ($1 == '-') {
          addCode("iconst_m1", lineNum);
          lineNum += 1;
          if ($2 == intNum){
            addCode("imul", lineNum);
            lineNum += 1;
          }
          else{
            addCode("fmul", lineNum);
            lineNum += 1;
          }
        }
    }
    | simple_expression ADDOP term
    {
        $$ = getFloatOrInt($1, $3);
        if($$ == intNum){
            addCode("i" + arithmeticMap[string(1, $2)], lineNum);
            lineNum += 1;
        }
        else{
            addCode("f" + arithmeticMap[string(1, $2)], lineNum);
            lineNum += 1;
        }
    }
    ;
term:
    factor
    | term MULOP factor
    {
        $$ = getFloatOrInt($1, $3);
        if($$ == intNum){
            addCode("i" + arithmeticMap[string(1, $2)], lineNum);
            lineNum += 1;
        }
        else{
            addCode("f" + arithmeticMap[string(1, $2)], lineNum);
            lineNum += 1;
        }
    }
    ;
factor:
    ID
    {
        if(!symbolTable.count(string($1)))
          yyerror("Undeclared variable.");
        $$ = symbolTable[string($1)].second;
        if(symbolTable[$1].second == intNum){
            addCode("iload_" + to_string(symbolTable[string($1)].first), lineNum);
            lineNum += 1;
        }
        else{
            addCode("fload_" + to_string(symbolTable[string($1)].first), lineNum);
            lineNum += 1;
        }
    }
    | IVAL
    {
        $$ = intNum;
        addCode("sipush " + string($1), lineNum);
        lineNum += 3;
    }
    | FVAL
    {
        $$ = floatNum;
        addCode("ldc " + string($1), lineNum);
        lineNum += 3;
    }
    | '(' simple_expression ')'
    {
        $$ = $2;
    }
    ;
sign:
    ADDOP
    ;
%%

int main(int, char**) {
    FILE *myfile = fopen("input_program.txt", "r");
    yyin = myfile;
    do {
        yyparse();
    } while (!feof(yyin));

    generateJavaByteCode();
}
void addCode(string s, int num){
    code.push_back(s);
    codeNumbers.push_back(num);

}
void generateJavaByteCode(){
    int i = 0;
    for(string s : code)
        out <<codeNumbers[i++] <<": " <<s << endl;
}
void backpatch(vector<int> *v, int a){
    if(v == nullptr) return;
    while((*v).size()){
        code[(*v).back()] += to_string(a);
        (*v).pop_back();
    }
}
void variableInitialization(char *id_val, int type){
    string str = string(id_val);
    if(symbolTable.count(str))
        yyerror("Multiple definitions of variable.");
    symbolTable[str] = make_pair(++varNum, (typeEnum) type);
}
int getFloatOrInt(int t1, int t2){
    return max(t1, t2);
}
void yyerror(const char *s) {
    cout << "parse error!  Message: " << s << endl;
    exit(-1);
}


vector<int>* combine(vector<int>*v1, vector<int>*v2) {
    while(v2->size()){
        int i = v2->back();
        v1->push_back(i);
        v2->pop_back();
    }
    return v1;
}




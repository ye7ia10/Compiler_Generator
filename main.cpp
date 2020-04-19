#include <iostream>
#include <vector>
#include <algorithm>
#include "lexical/include/LexicalAnalyzerGenerator.h"
#include "parser/Parser.h"
using namespace std;

int main()
{
/*

    LexicalAnalyzerGenerator lexicalAnalyzerGenerator;

    lexicalAnalyzerGenerator.NFAGenerator();

    lexicalAnalyzerGenerator.DFAMinizedGenerator();
    //lexicalAnalyzerGenerator.printTest();

    lexicalAnalyzerGenerator.printTest();

    ofstream myfile;
    myfile.open ("output.txt");




    lexicalAnalyzerGenerator.applyInput("input.txt");
    for (int i = 0; i < lexicalAnalyzerGenerator.tokenVec.size(); i++)
    {

        myfile <<  lexicalAnalyzerGenerator.tokenVec[i].toString() << endl;

    }

    myfile.close();

*/

    Parser *parser = new Parser("parser.txt");

    return 0;
}

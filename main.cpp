#include <iostream>
#include <vector>
#include <algorithm>
#include "lexical/include/LexicalAnalyzerGenerator.h"

using namespace std;

int main()
{


    LexicalAnalyzerGenerator lexicalAnalyzerGenerator;

    lexicalAnalyzerGenerator.NFAGenerator();

    lexicalAnalyzerGenerator.DFAMinizedGenerator();
    //lexicalAnalyzerGenerator.printTest();


    ofstream myfile;
    myfile.open ("output.txt");



    /* read input and apply lexical rule */
    lexicalAnalyzerGenerator.applyInput("input.txt");
    for (int i = 0; i < lexicalAnalyzerGenerator.tokenVec.size(); i++)
    {
        if (lexicalAnalyzerGenerator.tokenVec[i].isError())
        {
            myfile << "(Error)" << endl;
        }
        else
        {
            myfile <<  lexicalAnalyzerGenerator.tokenVec[i].toString() << endl;
        }
    }

    myfile.close();
    return 0;
}

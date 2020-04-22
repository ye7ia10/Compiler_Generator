#include "Parser.h"
#include "PredictiveTable.h"
Parser::Parser(string fileName)
{

    rules = new Rules();
    AnalyzeRules* analyze = new AnalyzeRules(fileName, rules);

    rules->removeLeftRecursion();
    rules->removeLeftFactoring();


    rules->calcFirst();
    rules->calcFollow();
    for (auto r : rules->getRules())
    {
        r.second->toString();
        for (Production* p : r.second->getProductions())
        {
            p->toString();
        }
        cout << "***********\n";

    }

    PredictiveTable* table = new PredictiveTable(rules);
}

void Parser::parsingLines(string startSymbol, PredictiveTable* predictiveTable)
{
    vector<Token> lineGroup;
    for (int i = 0; i < this->outputLexical.size() - 1;)
    {
        if (this->outputLexical[i].getLine() == this->outputLexical[i+1].getLine())
        {
            lineGroup.push_back(this->outputLexical[i]);
            i++;
        }
        else
        {
            lineGroup.push_back(this->outputLexical[i]);

            // Start the algorithm of LL(1) Parser
            stack<string> st;

            st.push("$");
            st.push(startSymbol);

            int tokenPointer = 0;
            vector<string> parsedOutput;
            bool errorFlag = false;

            while(st.size() != 1)
            {
                string x = st.top();

                std::unordered_map<std::string,int>::const_iterator exist = predictiveTable->terminals.find (x);

                if (exist != predictiveTable->terminals.end())
                {
                    if (x == "\'" + lineGroup[tokenPointer].toString() + "\'")
                    {
                        parsedOutput.push_back(lineGroup[tokenPointer].toString());
                        tokenPointer++;
                        st.pop();
                    }
                    else
                    {
                        /* error */
                        errorFlag = true;
                    }
                }
                else
                {
                    string tableEntry = predictiveTable->table[predictiveTable->nonTerminals[x]]
                                        [predictiveTable->terminals["\'" + lineGroup[tokenPointer].toString() + "\'"]];

                    if (tableEntry == "error")
                    {
                        /* error */
                        errorFlag = true;
                    }
                    else if (tableEntry == "Synch")
                    {

                    }
                    else
                    {
                        vector<string> splited = splitByChar(tableEntry, ' ');
                        for (int j = splited.size() - 1; j >= 0; j--)
                        {
                            if (splited[j] != " ")
                            {
                                st.push(splited[j]);
                            }

                        }
                        parsedOutput.push_back(tableEntry);
                    }
                }
            }
            // End of the algorithm

            string str = "";
            for (int j = 0; j < lineGroup.size(); j++)
            {
                str += lineGroup[j].toString() + " ";
            }

            ParsedLine parsedObj(str, parsedOutput, i, errorFlag);
            this->outputParser.push_back(parsedObj);

            lineGroup.clear();
            i++;
        }
    }

}

Parser::~Parser()
{
    //dtor
}

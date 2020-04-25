#include "Parser.h"
#include "PredictiveTable.h"
#include "ParsingAction.h"
Parser::Parser(string fileName, vector<Token> tokenVec)
{
    this->outputLexical = tokenVec;
    rules = new Rules();
    AnalyzeRules* analyze = new AnalyzeRules(fileName, rules);

    rules->removeLeftRecursion();
    //rules->removeLeftFactoring();


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

    this->table = new PredictiveTable(rules);

    startSymbol = rules->getFirstRule();
}

void Parser::parsingLines()
{
    cout << endl;
    cout << "=========== Starting Of Parsing Tracking ==========="<<endl;
    cout << endl;


    Token token("DollarSign", "$", -1, false);
    this->outputLexical.push_back(token);

    // Start the algorithm of LL(1) Parser
    stack<string> st;

    st.push("$");
    st.push(startSymbol);

    int tokenPointer = 0;
    vector<ParsingAction> parsedOutput;

    ParsingAction startParsing("StartSymbol", startSymbol);
    parsedOutput.push_back(startParsing);
    bool errorFlag = false;

    while(st.size() != 1)
    {
        string x = st.top();
        cout << "Top of the stack: " + x<<endl;

        std::unordered_map<std::string,int>::const_iterator exist;

        if (x == "\'=\'")
        {
            exist = table->terminals.find ("\'assign\'");
        }
        else
        {
            exist = table->terminals.find (x);
        }

        if (exist != table->terminals.end())
        {
            cout << x + " It is a terminal"<<endl;

            if (x == "\'=\'")
            {
                if ("\'assign\'" == "\'" + this->outputLexical[tokenPointer].toString() + "\'")
                {

                    if (this->outputLexical.size() > tokenPointer)
                    {
                        tokenPointer++;
                    }
                    st.pop();

                    cout << x + " is accepted"<<endl;
                    cout <<endl;
                }
                else
                {
                    /* error */
                    errorFlag = true;
                    st.pop();

                    ParsingAction parsingAction("ErrorTerminal", "Error: missing " + x);
                    parsedOutput.push_back(parsingAction);
                    cout << x + " is not equal " + "\'" + this->outputLexical[tokenPointer].toString() + "\'"<<endl;
                    cout <<endl;

                }
            }
            else
            {
                if (x == "\'" + this->outputLexical[tokenPointer].toString() + "\'")
                {

                    if (this->outputLexical.size() > tokenPointer)
                    {
                        tokenPointer++;
                    }
                    st.pop();

                    cout << x + " is accepted"<<endl;
                    cout <<endl;
                }
                else
                {
                    /* error */
                    errorFlag = true;
                    st.pop();

                    ParsingAction parsingAction("ErrorTerminal", "Error: missing " + x);
                    parsedOutput.push_back(parsingAction);
                    cout << x + " is not equal " + "\'" + this->outputLexical[tokenPointer].toString() + "\'"<<endl;
                    cout <<endl;
                }
            }

        }
        else
        {

            cout << x + " It is a non terminal"<<endl;

            string tableEntry;

            if (this->outputLexical[tokenPointer].toString() == "$")
            {
                tableEntry = this->table->table[this->table->nonTerminals[x]]
                             [this->table->terminals[this->outputLexical[tokenPointer].toString()]];
            }
            else
            {
                tableEntry = this->table->table[this->table->nonTerminals[x]]
                             [this->table->terminals["\'" + this->outputLexical[tokenPointer].toString() + "\'"]];
            }


            cout << "Table Entry: " + tableEntry<<endl;

            if (tableEntry == "Error")
            {
                /* error */
                errorFlag = true;



                if (this->outputLexical.size() > tokenPointer)
                {
                    ParsingAction parsingAction("ErrorTable", "Error:(illegal " + x + ") - discarded" + this->outputLexical[tokenPointer].toString());
                    parsedOutput.push_back(parsingAction);

                    tokenPointer++;
                    cout<< "Move the pointer of the input by 1"<< endl;
                    cout<< endl;
                }
                else
                {
                    ParsingAction parsingAction("Failed", "No Acceptance");
                    parsedOutput.push_back(parsingAction);
                    cout << "No way for acceptance" <<endl;
                    cout<< endl;
                    break;
                }


            }
            else if (tableEntry == "Synch")
            {
                st.pop();
                cout << "There is a Synch & Pop the top of the stack" <<endl;
                cout<< endl;
            }
            else
            {
                if (tableEntry == "\'\\L\'")
                {
                    st.pop();
                    ParsingAction parsingAction(x, tableEntry);
                    parsedOutput.push_back(parsingAction);
                    cout<< "Pop the non terminal"<<endl;
                    cout<<endl;
                }
                else
                {
                    st.pop();
                    vector<string> splited = splitByChar(tableEntry, ' ');
                    for (int j = splited.size() - 1; j >= 0; j--)
                    {
                        if (splited[j] != " ")
                        {
                            cout << "Pushed: " + splited[j]<<endl;
                            st.push(splited[j]);
                        }

                    }
                    ParsingAction parsingAction(x, tableEntry);
                    parsedOutput.push_back(parsingAction);
                    cout<< endl;
                }

            }
        }
    }

    cout << "End Parsing"<<endl;
    cout << endl;
    // End of the algorithm

    vector<string> printedOutput;

    cout<<parsedOutput[0].getProduction()<<endl;
    printedOutput.push_back(parsedOutput[0].getProduction());
    outputStore.push_back(parsedOutput[0].getProduction());

    for (int i = 1; i < parsedOutput.size(); i++)
    {
        if (parsedOutput[i].getRule() == "ErrorTerminal")
        {
            outputStore.push_back(parsedOutput[i].getProduction());
            cout<< parsedOutput[i].getProduction() <<endl;
        }
        else if (parsedOutput[i].getRule() == "ErrorTable")
        {
            outputStore.push_back(parsedOutput[i].getProduction());
            cout<< parsedOutput[i].getProduction() <<endl;
        }
        else if (parsedOutput[i].getRule() == "Failed")
        {
            outputStore.push_back(parsedOutput[i].getProduction());
            cout<< parsedOutput[i].getProduction() <<endl;
            break;
        }
        else
        {
            if (parsedOutput[i].getProduction() == "\'\\L\'")
            {
                string strBefore(printedOutput[printedOutput.size() - 1]);

                replace(strBefore, parsedOutput[i].getRule(), "");

                outputStore.push_back(strBefore);
                cout<<strBefore<<endl;

                printedOutput.push_back(strBefore);

            }
            else
            {
                string strBefore(printedOutput[printedOutput.size() - 1]);

                replace(strBefore, parsedOutput[i].getRule(), parsedOutput[i].getProduction());

                outputStore.push_back(strBefore);
                cout<<strBefore<<endl;

                printedOutput.push_back(strBefore);
            }
        }
    }


}

void Parser::replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return;
    str.replace(start_pos, from.length(), to);
}

Parser::~Parser()
{
    //dtor
}

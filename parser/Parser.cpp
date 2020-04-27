#include "Parser.h"
#include "PredictiveTable.h"
#include "ParsingAction.h"
#include <string>

Parser::Parser(string fileName, vector<Token> tokenVec)
{
    this->outputLexical = tokenVec;
    rules = new Rules();
    AnalyzeRules* analyze = new AnalyzeRules(fileName, rules);

    rules->removeLeftRecursion();
    rules->removeLeftFactoring();
    rules->calcFirst();
    rules->calcFollow();


    /*for (auto r : rules->getRules())
    {
        r.second->toString();
        for (Production* p : r.second->getProductions())
        {
            p->toString();
        }
        cout << "***********\n";

    }*/

    this->table = new PredictiveTable(rules);

    startSymbol = rules->getFirstRule();
}

void Parser::parsingLines()
{
    ofstream fout;
    fout.open("ParsingProcess.txt",ios::app);
    if(fout.is_open())
    {
        int line = 85;
        string start = "Starting Of Parsing Tracking";
        cout << endl;
        cout <<"||"<<string((line-start.length())/2,'=')<< left << setw(line)  <<  start + string((line-start.length())/2,'=')+"=||"<<endl;
        cout <<"||" << left<<setw(line) << ""<< "||"<<endl;
        fout << endl;
        fout <<"||"<<string((line-start.length())/2,'=')<< left << setw(line)  <<  start + string((line-start.length())/2,'=')+"=||"<<endl;
        fout <<"||" << left<<setw(line) << ""<< "||"<<endl;


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
            cout <<"||"<<left << setw(line)  << "Top of the stack: " + x<< "||" << endl;
            fout <<"||"<<left << setw(line)  << "Top of the stack: " + x<< "||" << endl;

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
                cout <<"||"<<left << setw(line) << x + " is a terminal" << "||" << endl;
                fout <<"||"<<left << setw(line) << x + " is a terminal" << "||" << endl;

                if (x == "\'=\'")
                {
                    if ("\'assign\'" == "\'" + this->outputLexical[tokenPointer].toString() + "\'")
                    {

                        if (this->outputLexical.size() > tokenPointer)
                        {
                            tokenPointer++;
                        }
                        st.pop();

                        cout <<"||"<<left << setw(line) << x + " is accepted"<<"||" << endl;
                        cout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                        fout <<"||"<<left << setw(line) << x + " is accepted"<<"||" << endl;
                        fout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                    }
                    else
                    {
                        /* error */
                        errorFlag = true;
                        st.pop();

                        ParsingAction parsingAction("ErrorTerminal", "Error: missing " + x);
                        parsedOutput.push_back(parsingAction);
                        cout <<"||"<<left << setw(line) << x + " is not equal " + "\'" + this->outputLexical[tokenPointer].toString() + "\'"<<"||"<<endl;
                        cout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                        fout <<"||"<<left << setw(line) << x + " is not equal " + "\'" + this->outputLexical[tokenPointer].toString() + "\'"<<"||"<<endl;
                        fout <<"||" << left<<setw(line) << ""<< "||"<<endl;

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

                        cout <<"||"<<left << setw(line) << x + " is accepted" << "||" <<endl;
                        cout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                        fout <<"||"<<left << setw(line) << x + " is accepted" << "||" <<endl;
                        fout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                    }
                    else
                    {
                        /* error */
                        errorFlag = true;
                        st.pop();

                        ParsingAction parsingAction("ErrorTerminal", "Error: missing " + x);
                        parsedOutput.push_back(parsingAction);
                        cout <<"||"<<left << setw(line) << x + " is not equal " + "\'" + this->outputLexical[tokenPointer].toString() + "\'"<<"||" <<endl;
                        cout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                        fout <<"||"<<left << setw(line) << x + " is not equal " + "\'" + this->outputLexical[tokenPointer].toString() + "\'"<<"||" <<endl;
                        fout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                    }
                }

            }
            else
            {

                cout <<"||"<<left << setw(line) << x + " is a non terminal"<<"||" <<endl;
                fout <<"||"<<left << setw(line) << x + " is a non terminal"<<"||" <<endl;

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


                cout <<"||"<<left << setw(line) << "Table Entry: " + tableEntry<<"||" << endl;
                fout <<"||"<<left << setw(line) << "Table Entry: " + tableEntry<<"||" << endl;

                if (tableEntry == "Error")
                {
                    /* error */
                    errorFlag = true;



                    if (this->outputLexical.size() > tokenPointer)
                    {
                        ParsingAction parsingAction("ErrorTable", "Error:(illegal " + x + ") - discarded" + this->outputLexical[tokenPointer].toString());
                        parsedOutput.push_back(parsingAction);

                        tokenPointer++;
                        cout <<"||"<<left << setw(line) << "Move the pointer of the input by 1"<<"||"<< endl;
                        cout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                        fout <<"||"<<left << setw(line) << "Move the pointer of the input by 1"<<"||"<< endl;
                        fout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                    }
                    else
                    {
                        ParsingAction parsingAction("Failed", "No Acceptance");
                        parsedOutput.push_back(parsingAction);
                        cout <<"||"<<left << setw(line) << "No way for acceptance" <<"||"<<endl;
                        cout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                        fout <<"||"<<left << setw(line) << "No way for acceptance" <<"||"<<endl;
                        fout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                        break;
                    }


                }
                else if (tableEntry == "Synch")
                {
                    st.pop();
                    cout <<"||"<<left << setw(line) << "There is a Synch & Pop the top of the stack" <<"||"<<endl;
                    cout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                    fout <<"||"<<left << setw(line) << "There is a Synch & Pop the top of the stack" <<"||"<<endl;
                    fout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                }
                else
                {
                    if (tableEntry == "\'\\L\'")
                    {
                        st.pop();
                        ParsingAction parsingAction(x, tableEntry);
                        parsedOutput.push_back(parsingAction);
                        cout <<"||"<<left << setw(line) << "Pop the non terminal"<<"||"<<endl;
                        cout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                        fout <<"||"<<left << setw(line) << "Pop the non terminal"<<"||"<<endl;
                        fout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                    }
                    else
                    {
                        st.pop();
                        vector<string> splited = splitByChar(tableEntry, ' ');
                        for (int j = splited.size() - 1; j >= 0; j--)
                        {
                            if (splited[j] != " ")
                            {
                                cout <<"||"<<left << setw(line) << "Pushed: " + splited[j]<<"||"<<endl;
                                fout <<"||"<<left << setw(line) << "Pushed: " + splited[j]<<"||"<<endl;
                                st.push(splited[j]);
                            }

                        }
                        ParsingAction parsingAction(x, tableEntry);
                        parsedOutput.push_back(parsingAction);
                        cout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                        fout <<"||" << left<<setw(line) << ""<< "||"<<endl;
                    }

                }
            }
        }
        string endL = "End Parsing";
        cout <<"||"<<string(((line-endL.length())/2),'=')<<left << setw(line) << "End Parsing"+string(((line-endL.length())/2),'=')+"||"<<endl;
        cout<<endl;
        fout <<"||"<<string(((line-endL.length())/2),'=')<<left << setw(line) << "End Parsing"+string(((line-endL.length())/2),'=')+"||"<<endl;
        fout <<endl;
        // End of the algorithm

        vector<string> printedOutput;

        cout <<parsedOutput[0].getProduction()<<endl;
        fout <<parsedOutput[0].getProduction()<<endl;
        printedOutput.push_back(parsedOutput[0].getProduction());
        outputStore.push_back(parsedOutput[0].getProduction());

        for (int i = 1; i < parsedOutput.size(); i++)
        {
            if (parsedOutput[i].getRule() == "ErrorTerminal")
            {
                outputStore.push_back(parsedOutput[i].getProduction());
                cout<< parsedOutput[i].getProduction() <<endl;
                fout<< parsedOutput[i].getProduction() <<endl;
            }
            else if (parsedOutput[i].getRule() == "ErrorTable")
            {
                outputStore.push_back(parsedOutput[i].getProduction());
                cout<< parsedOutput[i].getProduction() <<endl;
                fout<< parsedOutput[i].getProduction() <<endl;
            }
            else if (parsedOutput[i].getRule() == "Failed")
            {
                outputStore.push_back(parsedOutput[i].getProduction());
                cout<< parsedOutput[i].getProduction() <<endl;
                fout<< parsedOutput[i].getProduction() <<endl;
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
                    fout<<strBefore<<endl;

                    printedOutput.push_back(strBefore);

                }
                else
                {
                    string strBefore(printedOutput[printedOutput.size() - 1]);

                    replace(strBefore, parsedOutput[i].getRule(), parsedOutput[i].getProduction());

                    outputStore.push_back(strBefore);
                    cout<<strBefore<<endl;
                    fout<<strBefore<<endl;

                    printedOutput.push_back(strBefore);
                }
            }
        }
    fout.close();
    }
}

void Parser::replace(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return;
    str.replace(start_pos, from.length(), to);
}

Parser::~Parser()
{
    //dtor
}

#include "Rules.h"
#include "Rule.h"
#include <string>
#include <string.h>

Rules::Rules()
{
    //ctor
}

Rules::~Rules()
{
    //dtor
}

void Rules::removeLeftRecursion()
{
    int InitialNonTerminals = rules.size();
    unordered_map<string, Rule*>::iterator it;
    unordered_map<string, Rule*>::iterator InnerIt;
    int i = 0, j = 0;
    for ( it = rules.begin(); it != rules.end(), i < InitialNonTerminals; it++ )
    {
        string CurNonTerm = it -> first;
        Rule *currentRule = it -> second;
        vector<string> currentRuleProd = currentRule->getProductionsString();
        for (InnerIt = rules.begin(); j < i ; InnerIt++)  /*** iterate on the previous ***/
        {
            string TempNonTerm = InnerIt -> first;
            Rule *TempRule = InnerIt -> second;
            vector<string> TempRuleProd = TempRule->getProductionsString();
            currentRuleProd = replaceNons(currentRuleProd,TempNonTerm, TempRuleProd);
            j++;
        }
        if (hasLeftRecusrion(CurNonTerm, currentRuleProd))
        {
            //remove left Recursion
            EliminateLRCurrenRule(currentRuleProd, CurNonTerm);
        }
        i++;
    }
}
void Rules::EliminateLRCurrenRule(vector<string>currentRuleProd, string curNon){

    vector<string> alphas;
    vector<string> betas;
    for (int i = 0; i < currentRuleProd.size() ; i++){
        string firstWord = currentRuleProd[i].substr(0, currentRuleProd[i].find(" "));
        if (firstWord == curNon){
            alphas.push_back(currentRuleProd[i].substr(currentRuleProd[i].find_first_of(" \t")+1)
                             +  " " +curNon + "`");
        } else {
            if (firstWord != "\L"){
                betas.push_back(currentRuleProd[i] + " " +curNon + "`");
            } else {
                betas.push_back("\\L");
            }
        }
    }
    alphas.push_back("\\L");
    Rule CurModified(curNon, betas);    /*** after modify the RHS of current Rule ***/
    Rule NewRule (curNon + "`", alphas);   /*** new Rule for the dashed Non-Terminal ***/
    rules.insert(pair<string, Rule*>(curNon,&CurModified));    /*** pointer a7o mali4 da3wa ***/
    rules.insert(pair<string, Rule*>(curNon + "`",&NewRule));
}
bool Rules::hasLeftRecusrion(string CurNonTerm, vector<string> currentRuleProd)
{
    for (int i = 0; i < currentRuleProd.size() ; i++)
    {
        string firstWord = currentRuleProd[i].substr(0, currentRuleProd[i].find(" "));
        if (CurNonTerm == firstWord)
        {
            return true;
        }
    }
    return false;
}

vector<string> Rules::replaceNons (vector<string> curProd, string tempName, vector<string> tempProd)
{
    vector<string> res;
    for (int i = 0; i < curProd.size() ; i++)
    {
            string firstWord = curProd[i].substr(0, curProd[i].find(" "));
            if (firstWord == tempName)
            {
                if (curProd[i].size() > 1)
                {
                    string sentence= " " + curProd[i].substr(curProd[i].find_first_of(" \t")+1);
                    for (int j = 0; j < tempProd.size() ; j++)
                    {
                        if (tempProd[i] != "\L") /*** if not epsilon ***/
                        {
                            res.push_back(tempProd[i] + sentence);
                        }
                        else  /*** epsilon ***/
                        {
                            res.push_back(curProd[i].substr(curProd[i].find_first_of(" \t")+1));
                        }
                    }
                }
                else
                {
                    for (int j = 0; j < tempProd.size() ; j++)
                    {
                        res.push_back(tempProd[i]);
                    }
                }
        }
        else
        {
            res.push_back(curProd[i]);
        }
    }
    return res;
  }

  void Rules::addRule(Rule* rule) {
        rules[rule->getName()] = rule;
        NonTerminalsNames.push_back(rule->getName());
  }

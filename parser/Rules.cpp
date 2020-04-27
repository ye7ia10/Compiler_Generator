#include "Rules.h"
#include "Rule.h"
#include <string>
#include <string.h>
const string epsilon = "\'\\L\'";
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
    map<string, Rule*>::iterator it;
    map<string, Rule*>::iterator InnerIt;
    map<string, Rule*> newMap;
    int i = 0, j = 0;
    for ( it = rules.begin(); it != rules.end(), i < InitialNonTerminals; it++ )
    {
        string CurNonTerm = it -> first;
        Rule *currentRule = it -> second;
        vector<string> currentRuleProd = currentRule->getProductionsString();

        j = 0;
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

            EliminateLRCurrenRule(currentRuleProd, CurNonTerm, newMap);
        }
        else
        {
            Rule* NewRule = new Rule(CurNonTerm, currentRuleProd);
            rules[CurNonTerm] = NewRule;
            newMap[CurNonTerm] = NewRule;
        }
        i++;

    }
    rules = newMap;
}
void Rules::EliminateLRCurrenRule(vector<string>currentRuleProd, string curNon, map<string, Rule*> &newMap)
{

    vector<string> alphas;
    vector<string> betas;

    for (int i = 0; i < currentRuleProd.size() ; i++)
    {

        string firstWord = currentRuleProd[i].substr(0, currentRuleProd[i].find(" "));
        if (firstWord == curNon)
        {
            alphas.push_back(currentRuleProd[i].substr(currentRuleProd[i].find_first_of(" \t")+1)
                             +  " " +curNon + "`");
        }
        else
        {
            if (firstWord != epsilon)
            {
                betas.push_back(currentRuleProd[i] + " " +curNon + "`");
            }
            else
            {
                betas.push_back(epsilon);
            }
        }
    }
    alphas.push_back(epsilon);
    Rule* CurModified = new Rule(curNon, betas);    /*** after modify the RHS of current Rule ***/
    Rule* NewRule = new Rule(curNon + "`", alphas);   /*** new Rule for the dashed Non-Terminal ***/
    rules[curNon] = CurModified;
    newMap[curNon] = CurModified;
    newMap[curNon + "`"] = NewRule;
    //rules[curNon + "`"] = NewRule;

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

            if (curProd[i].size() > firstWord.size())
            {
                string sentence= " " + curProd[i].substr(curProd[i].find_first_of(" \t")+1);
                for (int j = 0; j < tempProd.size() ; j++)
                {
                    if (tempProd[j] != epsilon) /*** if not epsilon ***/
                    {
                        res.push_back(tempProd[j] + sentence);
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
                    res.push_back(tempProd[j]);
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

void Rules::removeLeftFactoring()
{
    map <string, Rule*>::iterator it;

    it = rules.begin();

    while (it != rules.end())
    {
        vector<string> productionString = it->second->getProductionsString();
        sort(productionString.begin(), productionString.end());
        vector<vector<string>> splited;
        string prefix;
        if (productionString.size() == 1)
        {
        }
        else
        {
            int searchEnd = getLastindex(productionString,0,productionString.size());
            for(int i=0 ; i<productionString.size() ; i++)
            {
                vector<string> temp = splitByChar(productionString[i],' ');
                splited.push_back(temp);
                //cout << productionString[i]<<endl;
            }
            int searchStart = 0;
            while (searchStart < productionString.size())
            {
                //cout << "a:" << searchStart << endl;

                for (int i=searchStart+1 ; i<=searchEnd ; i++)
                {
                    if(splited[searchStart][0] != splited[i][0])
                        searchStart++;
                    else
                        break;
                }

                int prefixNo = 0;
                prefix = splited[searchStart][0];
                for(int i=searchStart ; i<searchEnd ; i++)
                {
                    if(splited[i][0] == splited[i+1][0])
                        prefixNo++;
                    else
                        break;
                }
                if(prefixNo==0)
                {
                    prefix = "";
                }
                otherOccurences(splited,prefix,searchStart,prefixNo,1);

                int matchbegin = searchStart;
                int matchEnd = searchStart + prefixNo;
                //cout << "prefix: " << prefix << endl << "begin: " << matchbegin << endl << "End: " << matchEnd << endl << "*******************" << endl;
                searchStart = searchEnd + 1;
                searchEnd = getLastindex(productionString,searchStart,productionString.size());
                //cout << searchStart<<endl;
                //cout << searchEnd<<endl;
            }
        }
        if(prefix.length()>0)
        {
            modifyRule(productionString,prefix,it->second);
            addRuleLeftFactoring(productionString,prefix,it->first,it->second);
        }

        /*cout << "Non Terminals Symbols:" << endl;
        for (int i = 0; i < this->NonTerminalsNames.size(); i++)
        {
            cout<< NonTerminalsNames[i]<<endl;
        }

        //cout<<endl;
        //cout<< "RulesLF: " <<endl;
        map <string, Rule*>::iterator i;

        for (i = rules.begin(); i != rules.end(); i++)
        {
            cout<<endl;

            cout<< i->first + "-> ";

            for (int j = 0; j < i->second->productions.size(); j++)
            {
                if (j != 0)
                {
                    cout<< "| ";
                }
                for (int x = 0; x < i->second->productions[j]->elements.size(); x++)
                {
                    cout<< i->second->productions[j]->elements[x]->getName() + " ";
                }
            }
        }
        cout<<endl;
        cout<<endl;*/
        it++;

    }
}

void Rules::modifyRule(vector<string> &modifiedRule, string prefix, Rule* currentRule)
{

    for (int i = 0; i < modifiedRule.size(); i++)
    {

        if (modifiedRule[i].length() >= prefix.length() && modifiedRule[i] != epsilon)
        {

            string s = modifiedRule[i];

            std::string::size_type y = s.find(prefix);

            if (i != std::string::npos)
            {
                modifiedRule[i].erase(y, prefix.length());
            }

            if (modifiedRule[i].length() == 0)
            {
                modifiedRule[i] = epsilon;
            }
        }
    }
    currentRule->setProductionString(modifiedRule);
}

void Rules::addRuleLeftFactoring(vector<string> &modifiedRule, string prefix, string nonTerminal, Rule* currentRule)
{

    std::vector<string>::iterator it = std::find(NonTerminalsNames.begin(), NonTerminalsNames.end(), nonTerminal);
    int index = std::distance(NonTerminalsNames.begin(), it);

    string oldNonTermianl = this->NonTerminalsNames[index];

    string newNonTerminal = this->NonTerminalsNames[index] + "`";

    while (true)
    {
        if (std::find(this->NonTerminalsNames.begin(), this->NonTerminalsNames.end(),
                      newNonTerminal) == this->NonTerminalsNames.end())
        {
            break;
        }
        else
        {
            newNonTerminal = newNonTerminal + "`";
        }
    }

    currentRule->setName(newNonTerminal);

    vector<Production*> newProductions;
    for (int i = 0; i < modifiedRule.size(); i++)
    {
        Production* production = new Production(modifiedRule[i]);
        newProductions.push_back(production);
    }

    currentRule->setProductions(newProductions);
    this->addRule(currentRule);

    Rule* rule = new Rule(oldNonTermianl, prefix + " " + newNonTerminal);
    this->addRule(rule);

}


bool Rules::mycomp(string a, string b)
{
    return a<b;
}

void Rules::addRule(Rule* rule)
{
    rules[rule->getName()] = rule;

    if(std::find(NonTerminalsNames.begin(), NonTerminalsNames.end(), rule->getName()) == NonTerminalsNames.end())
    {
        NonTerminalsNames.push_back(rule->getName());
    }

}

vector<RuleComponent*>Rules::calcFirstByRec(Rule* r)
{


    vector<RuleComponent*> allF;
    for (Production* p : r->getProductions())
    {

        if(p->getRlueComponent(0)->isTerminal())
        {
            allF.push_back(p->getRlueComponent(0));
            p->addFirst({p->getRlueComponent(0)});
        }
        else
        {
            Rule* ruleOfFollow = rules[p->getRlueComponent(0)->getName()];
            vector<RuleComponent*> subFirst = calcFirstByRec(ruleOfFollow);
            p->addFirst(subFirst);
            allF.insert(allF.end(), subFirst.begin(), subFirst.end());
        }
    }
    return allF;
}
void Rules::calcFirst()
{
    for (auto r : rules)
    {
        Rule* rule = r.second;
        if (rule->getFirst().size() == 0)
        {

            //cout<<rule->getName()<<endl;
            vector<RuleComponent*> c = calcFirstByRec(rule);
            //cout<<rule->getName()<<endl;
            r.second->putFirst(c);
        }
    }
}
map <string, Rule*>  Rules::getRules()
{
    return rules;
}

void Rules::calcFollow()
{
    addDollarToFirstRule();
    map<string, Rule*>::iterator it;
    for (it = rules.begin(); it != rules.end() ; it++)
    {
        string nonTe = it->first;
        if (visitedFollow[nonTe])
        {
            continue;
        }
        set<string>s;

        getFollow(it, nonTe, s);

        visitedFollow[nonTe] = true;
    }
}

void Rules::addDollarToFirstRule()
{
    map<string, Rule*>::iterator it;
    for (it = rules.begin(); it != rules.end() ; it++)
    {
        if (it->first == getFirstRule())   /*** if first non terminal push dollar sign ***/
        {
            Rule *currentRule = it -> second;
            RuleComponent* component = new RuleComponent("$");
            currentRule ->addFollow(component);
            if(find(terminalNames.begin(),terminalNames.end(),component->getName()) == terminalNames.end())
                terminalNames.push_back(component->getName());
        }
    }
}

void Rules::getFollow(map<string, Rule*>::iterator it, string nonTe, set<string>& vis)
{

    if (vis.find(nonTe) != vis.end())
    {
        return;
    }
    vis.insert(nonTe);
    if (nonTe == getFirstRule())   /*** if first non terminal push dollar sign ***/
    {
        Rule *currentRule = it -> second;
        RuleComponent* component = new RuleComponent("$");
        currentRule ->addFollow(component);
        if(find(terminalNames.begin(),terminalNames.end(),component->getName()) == terminalNames.end())
            terminalNames.push_back(component->getName());
    }
    map<string, Rule*>::iterator itMap;
    for (itMap = rules.begin(); itMap != rules.end() ; itMap++)
    {
        vector<Production*> Productions = itMap->second->getProductions();
        for (int i = 0; i < Productions.size() ; i++)
        {
            vector<RuleComponent*> components = Productions[i]-> elements;
            for (int j = 0; j < components.size() ; j++)
            {
                if (components[j]->getName() == nonTe)
                {
                    if (j == components.size()-1 && nonTe != itMap->first)
                    {
                        getFollow(it, itMap->first, vis);
                    }
                    else if (j != components.size()-1)
                    {
                        set<string>newVis;
                        getFollowByFirst(components[j+1], j, it, itMap,components, newVis);
                    }
                }
            }
        }
    }
    vis.erase(nonTe);
}
void Rules::getFollowByFirst(RuleComponent* nextComponent, int idxComponent,
                             map<string, Rule*>::iterator it, map<string, Rule*>::iterator itMap,
                             vector<RuleComponent*> components, set<string>& vis)
{


    if (vis.find(components[idxComponent]->getName()) != vis.end())
    {
        return;
    }
    vis.insert(components[idxComponent]->getName());

    if (nextComponent->isTerminal())
    {
        Rule *currentRule = it -> second;
        currentRule ->addFollow(nextComponent);
        if(find(terminalNames.begin(),terminalNames.end(),nextComponent->getName()) == terminalNames.end())
            terminalNames.push_back(nextComponent->getName());
    }
    else
    {
        vector<RuleComponent*> nextComponentFirst = rules[nextComponent->getName()]->getFirst();
        for (int i = 0; i < nextComponentFirst.size() ; i++)
        {
            if (nextComponentFirst[i]->getName() != epsilon)
            {
                Rule *currentRule = it -> second;
                currentRule ->addFollow(nextComponentFirst[i]);
                if(find(terminalNames.begin(),terminalNames.end(),nextComponentFirst[i]->getName()) == terminalNames.end())
                    terminalNames.push_back(nextComponentFirst[i]->getName());
            }
            else
            {
                if (idxComponent+2 == components.size())
                {

                    getFollow(it, itMap->first, vis);
                }
                else
                {
                    getFollowByFirst(components[idxComponent+2], idxComponent+1, it, itMap, components, vis);
                }
            }
        }
    }
    vis.erase(components[idxComponent]->getName());
}


void Rules::setFirstRule(string non)
{
    this->firstRuleString = non;
}

string Rules::getFirstRule()
{
    return this->firstRuleString;
}
vector<string>Rules::getTerminalNames()
{
    return terminalNames;
}
vector<string>Rules::getNonTerminalNames()
{
    return NonTerminalsNames;

}
int Rules::getLastindex (vector<string> &s, int start, int arrSize)
{
    int arrEnd = arrSize-1;
    while (start < arrSize && s[start][0] != s[arrEnd][0])
    {
        arrEnd--;
    }
    return arrEnd;
}
void Rules::otherOccurences (vector<vector<string>> &splited, string &prefix, int searchStart, int prefixNo, int compNo)
{
    bool found = false;

    for (int i=searchStart ; i<=searchStart+prefixNo-1 ; i++)
    {
        if(splited[i][compNo] == splited[i+1][compNo])
        {
            found = true;
        }
        else
        {
            found = false;
            break;
        }
    }
    if (found)
    {
        prefix = prefix + splited[searchStart][compNo];
        otherOccurences(splited,prefix,searchStart,prefixNo,compNo+1);
    }
}

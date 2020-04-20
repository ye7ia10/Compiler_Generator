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
        } else {
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
        bool iteratorUpdated = false;
        vector<string> productionString = it->second->getProductionsString();

        for (int i = 0; i < productionString.size(); i++)
        {
            productionString[i] = std::regex_replace(productionString[i], std::regex("^\\s+"), std::string(""));
        }

        sort(productionString.begin(), productionString.end());

        for (int i = 0; i < productionString.size(); i++)
        {
            cout<<productionString[i] + " | ";
        }
        cout<<endl;

        if (productionString.size() == 1)
        {
        }
        else
        {
            string prefix = commonPrefix(productionString, 0, productionString.size());

            cout<< prefix<<endl;

            if (prefix[0] == '\'' && prefix[1] == ' ')
            {
                prefix = prefix.substr(2, prefix.length());
            }
            else if (prefix[prefix.length() - 1] == '\'' && prefix[prefix.length() - 2] == ' ')
            {
                prefix = prefix.substr(0, prefix.length() - 1);
            }

            if (prefix != "" && prefix != epsilon && prefix != " " && prefix != "\'")
            {
                modifyRule(productionString, prefix, it->second);
                addRuleLeftFactoring(productionString, prefix, it->first, it->second);
            }
            else
            {

                for (int i = 0; i < productionString.size() - 1; i++)
                {
                    bool flag = false;
                    for (int j = productionString.size() - 1; j > i; j--)
                    {

                        string prefixString = commonPrefix(productionString, i, j + 1);

                        cout<< prefixString <<endl;

                        if (prefixString[0] == '\'' && prefixString[1] == ' ')
                        {
                            prefixString = prefixString.substr(2, prefixString.length());
                        }
                        else if (prefixString[prefixString.length() - 1] == '\'' && prefixString[prefixString.length() - 2] == ' ')
                        {
                            prefixString = prefixString.substr(0, prefixString.length() - 1);
                        }

                        if (prefixString != "" && prefixString != epsilon && prefixString != " " && prefixString != "\'")
                        {
                            partialModifiedRule(productionString, prefixString, it->first, it->second);
                            flag = true;
                            break;
                        }
                    }
                    if (flag)
                    {
                        iteratorUpdated = true;

                        if (it == rules.begin())
                        {
                        }
                        else
                        {
                            advance(it, -1);
                        }
                        break;
                    }
                }
            }


        }
        if (!iteratorUpdated)
        {
            it++;
        }
    }

    cout<<endl;
    for (int i = 0; i < this->NonTerminalsNames.size(); i++)
    {
        cout<< NonTerminalsNames[i]<<endl;
    }

    cout<<endl;
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

}

int Rules::findMinLength(vector<string> arr, int startIndex, int endIndex)
{
    int min = INT_MAX;

    for (int i = startIndex; i < endIndex; i++)
        if (arr[i].length() < min)
            min = arr[i].length();

    return (min);
}

bool Rules::allContainsPrefix(vector<string> arr, string str,
                              int start, int end, int startIndex, int endIndex)
{
    for (int i = startIndex; i < endIndex; i++)
    {
        for (int j = start; j <= end; j++)
            if (arr[i][j] != str[j])
            {
                return false;
            }
    }

    return true;
}

string Rules::commonPrefix(vector<string> arr, int startIndex, int endIndex)
{

    int index = findMinLength(arr, startIndex, endIndex);
    string prefix = ""; // Our resultant string

    // We will do an in-place binary search on the
    // first string of the array in the range 0 to
    // index
    int low = 0, high = index;

    while (low <= high)
    {
        // Same as (low + high)/2, but avoids overflow
        // for large low and high
        int mid = low + (high - low) / 2;

        if (allContainsPrefix(arr, arr[startIndex], low, mid, startIndex, endIndex))
        {
            // If all the strings in the input array contains
            // this prefix then append this substring to
            // our answer
            prefix = prefix + arr[startIndex].substr(low, mid - low + 1);

            //cout<< prefix << endl;

            // And then go for the right part
            low = mid + 1;
        }

        else // Go for the left part
            high = mid - 1;
    }

    return (prefix);
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

    this->NonTerminalsNames[index] = this->NonTerminalsNames[index] + "\'";

    while (true)
    {
        if (std::find(this->NonTerminalsNames.begin(), this->NonTerminalsNames.end(),
                      this->NonTerminalsNames[index]) == this->NonTerminalsNames.end())
        {
            this->NonTerminalsNames[index] = this->NonTerminalsNames[index] + "\'";
        }
        else
        {
            break;
        }
    }

    currentRule->setName(this->NonTerminalsNames[index]);

    vector<Production*> newProductions;
    for (int i = 0; i < modifiedRule.size(); i++)
    {
        Production* production = new Production(modifiedRule[i]);
        newProductions.push_back(production);
    }

    currentRule->setProductions(newProductions);
    this->addRule(currentRule);

    Rule* rule = new Rule(oldNonTermianl, prefix + " " + this->NonTerminalsNames[index]);
    this->addRule(rule);

}

void Rules::partialModifiedRule(vector<string> &modifiedRule, string prefix, string nonTerminal, Rule* currentRule)
{

    std::vector<string>::iterator it = std::find(NonTerminalsNames.begin(), NonTerminalsNames.end(), nonTerminal);
    int index = std::distance(NonTerminalsNames.begin(), it);

    vector<string> newRule;

    int flag = 0;

    string newNonTerminal = this->NonTerminalsNames[index] + "\'";

    while (true)
    {
        if (std::find(this->NonTerminalsNames.begin(), this->NonTerminalsNames.end(),
                      newNonTerminal) == this->NonTerminalsNames.end())
        {
            break;
        }
        else
        {
            newNonTerminal = newNonTerminal + "\'";

        }
    }


    for (int i = 0; i < modifiedRule.size(); i++)
    {

        if (modifiedRule[i].length() >= prefix.length() && modifiedRule[i] != epsilon)
        {

            string s = modifiedRule[i];

            std::string::size_type y = s.find(prefix);

            if (i != std::string::npos && prefix == modifiedRule[i].substr(0, prefix.length()))
            {
                if (flag == 0)
                {
                    newRule.push_back(modifiedRule[i].substr(prefix.length(), modifiedRule[i].length()));
                    modifiedRule[i].erase(prefix.length(), modifiedRule[i].length());
                    modifiedRule[i] = modifiedRule[i] + " " + newNonTerminal;

                    flag++;
                }
                else
                {
                    //cout << prefix + "=======" + modifiedRule[i]<<endl;
                    newRule.push_back(modifiedRule[i].substr(prefix.length(), modifiedRule[i].length()));
                    modifiedRule.erase(modifiedRule.begin() + i);
                    i--;
                }
            }

            if (modifiedRule[i].length() == 0)
            {
                modifiedRule[i] = epsilon;
            }
        }
    }

    currentRule->setProductionString(modifiedRule);

    //cout << modifiedRule.size()<< endl;

    vector<Production*> newProductions;
    for (int i = 0; i < modifiedRule.size(); i++)
    {
        cout<<modifiedRule[i] + " "<<endl;
        Production* production = new Production(modifiedRule[i]);
        newProductions.push_back(production);
    }

    currentRule->setProductions(newProductions);

    string newRuleStr = "";
    for (int i = 0; i < newRule.size(); i++)
    {
        //cout<<newRule[i]<<endl;
        if (i == 0)
        {
            newRuleStr += newRule[i];
            //cout<<newRuleStr<<endl;
        }
        else
        {
            newRuleStr += "| " + newRule[i];
        }
    }

    cout<<newRuleStr<<endl;

    Rule* rule = new Rule(newNonTerminal, newRuleStr);
    this->addRule(rule);

}

bool Rules::mycomp(string a, string b)
{
    return a<b;
}

void Rules::addRule(Rule* rule)
{
    rules[rule->getName()] = rule;
    NonTerminalsNames.push_back(rule->getName());
}

vector<RuleComponent*>Rules::calcFirstByRec(Rule* r) {


    vector<RuleComponent*> allF;
    for (Production* p : r->getProductions()) {
        if(p->getRlueComponent(0)->isTerminal()) {
            allF.push_back(p->getRlueComponent(0));
            p->addFirst({p->getRlueComponent(0)});
        } else {

            Rule* ruleOfFollow = rules[p->getRlueComponent(0)->getName()];
            vector<RuleComponent*> subFirst = calcFirstByRec(ruleOfFollow);
            p->addFirst(subFirst);
            allF.insert(allF.end(), subFirst.begin(), subFirst.end());
        }
    }
    return allF;
}
void Rules::calcFirst() {
    for (auto r : rules) {
        if (r.second->getFirst().size() == 0) {

            vector<RuleComponent*> c = calcFirstByRec(r.second);
            r.second->putFirst(c);
        }
    }
}
map <string, Rule*>  Rules::getRules() {
    return rules;
}

void Rules::calcFollow(){
    map<string, Rule*>::iterator it;
    for (it = rules.begin(); it != rules.end() ; it++){
        string nonTe = it->first;
        if (visitedFollow[nonTe]){
            continue;
        }
        set<string>s;

        getFollow(it, nonTe, s);

        visitedFollow[nonTe] = true;
    }
}

void Rules::getFollow(map<string, Rule*>::iterator it, string nonTe, set<string>& vis){

    if (vis.find(nonTe) != vis.end()) {
        return;
    }
    vis.insert(nonTe);
    map<string, Rule*>::iterator Fit;
    Fit = rules.begin();
    if (Fit-> first == nonTe){  /*** if first non terminal push dollar sign ***/
          Rule *currentRule = it -> second;
          RuleComponent* component = new RuleComponent("$");
          currentRule ->addFollow(component);
    }
    map<string, Rule*>::iterator itMap;
    for (itMap = rules.begin(); itMap != rules.end() ; itMap++){
        vector<Production*> Productions = itMap->second->getProductions();
        for (int i = 0; i < Productions.size() ; i++){
            vector<RuleComponent*> components = Productions[i]-> elements;
            for (int j = 0; j < components.size() ; j++){
                if (components[j]->getName() == nonTe){
                    if (j == components.size()-1 && nonTe != itMap->first){
                         getFollow(it, itMap->first, vis);
                    } else if (j != components.size()-1) {
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
        vector<RuleComponent*> components, set<string>& vis){


    if (vis.find(components[idxComponent]->getName()) != vis.end()) {
        return;
    }
    vis.insert(components[idxComponent]->getName());

    if (nextComponent->isTerminal()){
        Rule *currentRule = it -> second;
        currentRule ->addFollow(nextComponent);
    } else {
         vector<RuleComponent*> nextComponentFirst = rules[nextComponent->getName()]->getFirst();
         for (int i = 0; i < nextComponentFirst.size() ; i++){
            if (nextComponentFirst[i]->getName() != epsilon){
                Rule *currentRule = it -> second;
                currentRule ->addFollow(nextComponentFirst[i]);
            } else {
                if (idxComponent+2 == components.size()){

                    getFollow(it, itMap->first, vis);
                } else {
                    getFollowByFirst(components[idxComponent+2], idxComponent+1 , it, itMap, components, vis);
                }
            }
         }
    }
    vis.erase(components[idxComponent]->getName());
}

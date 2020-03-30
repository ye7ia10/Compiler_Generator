#include "../include/RegularParser.h"

RegularParser::RegularParser()
{
    //ctor
}

RegularParser::~RegularParser()
{
    //dtor
}
void RegularParser::parseFile(vector<string>s)
{
    /* clear all data if object used in parsing previous file */
    nfaVector.clear();
    regDefinition.clear();
    totalNFA.clear();
    tansitionsMapping.clear();
    tansitionsMappingForPrint.clear();
    finalStates.clear();
    finalStatesPriority.clear();
    finalStatesName.clear();
    /* parsing every line */
    for (auto line : s)
    {
        parseLine(line);
    }
    /*************** Here we finished NFA for each rule ***************/
    prepareForDfa();


    return;
}


void RegularParser::prepareForDfa() {

    int added = 1, number = 0;
    /* loop throw every state and numerate the inputs and priority for each NFA */
    for (int i = 0; i < nfaVector.size(); i++)
    {

        int newFinalState = nfaVector[i].finalState + added;
        finalStates.push_back(to_string(newFinalState));
        finalStatesPriority[newFinalState] = nfaVector[i].priority;
        finalStatesName.push_back(nfaVector[i].name);

        nfaVector[i].addNumberToTransitions(added);
        added = added + nfaVector[i].getStateTable().size();
        priorityToRule[nfaVector[i].priority] = nfaVector[i].name;

        for (int j = 0; j < nfaVector[i].getStateTable().size(); j++)
        {
            for(auto it : nfaVector[i].getStateTable()[j].getInputsWithTranstions())
            {

                if (tansitionsMapping.find(it.first) == tansitionsMapping.end() && it.first != ' ')
                {
                    tansitionsMapping[it.first] = number;
                    tansitionsMappingForPrint[number] = it.first;
                    number++;
                    string str = "";
                    str += it.first;
                    inputsTags.push_back(str);
                }
            }
        }
    }

    /* add epsilon to inputs */
    tansitionsMapping[' '] = number;
    tansitionsMappingForPrint[number] = ' ';
    inputsTags.push_back(" ");

    added = 1;
    State start;
    for (int i = 0; i < nfaVector.size(); i++)
    {
        string c = to_string(added);
        start.addTransition(c, ' ');
        added = added + nfaVector[i].getStateTable().size();
    }
    totalNFA.push_back(start);
    int row = 1;
    for (int i = 0; i < nfaVector.size(); i++)
    {
        for (int j = 0; j < nfaVector[i].getStateTable().size(); j++)
        {
            totalNFA.push_back(nfaVector[i].getStateTable()[j]);
            row++;
        }
    }


    finalStatesNameOrdered = finalStates;

    /* sort state names according to its periority */
    sort(finalStatesNameOrdered.begin(), finalStatesNameOrdered.end(), [this](string& first, string second)
    {
        int a1 = finalStatesPriority[stoi(first)];
        int a2 = finalStatesPriority[stoi(second)];
        if (a1 < a2)
        {
            return true;
        }
        return false;
    });

}
void RegularParser::parseLine(string regexString)
{
    trim(regexString);

    if (regexString.size() < 2)
    {
        return;
    }
    /* Keywords */
    if (regexString[0] == '{' && regexString[regexString.size() - 1] == '}')
    {
        vector<string>v = splitByChar(regexString.substr(1, regexString.size() - 2), ' ');
        for (int i = 0; i < v.size(); i++)
        {
            addStringToNFA(v[i]);
        }
    }
    /* Punctuations */
    if (regexString[0] == '[' && regexString[regexString.size() - 1] == ']')
    {
        for (int i = 0; i < regexString.size(); i++)
        {
            if (i + 1 < regexString.size() && regexString[i] == '\\' && regexString[i + 1] == '\\')
            {
                continue;
            }
            if (regexString[i] == '\\')
                regexString[i] = ' ';
        }
        vector<string>v = splitByChar(regexString.substr(1, regexString.size() - 2), ' ');
        for (int i = 0; i < v.size(); i++)
        {
            addStringToNFA(v[i]);
        }
    }
    string name = "";
    for (int i = 0; i < regexString.size(); i++)
    {
        if (regexString[i] == '=')
        {
            /* Regular definitions */
            DataEntry regular = getNFA(regexString.substr(i + 1, regexString.size() - i));
            if (!regular.haveError)
            {
                trim(name);
                regDefinition[name] = regular.nfa;
            }
            break;
        }
        else if (regexString[i] == ':')
        {
            /* Regular expressions */
            DataEntry regular = getNFA(regexString.substr(i + 1, regexString.size() - i));
            if (!regular.haveError)
            {
                trim(name);
                regular.nfa.priority = priority;
                priority = priority + 1;
                regular.nfa.name = name;
                nfaVector.push_back(regular.nfa);
            }
            break;
        }

        name += regexString[i];

    }


}

vector<string>RegularParser::split(string regexString)
{

    addNeededSpace(regexString, '-'); /*   add spaces before and after - to make it easy to determine if it is operation or charachter   */
    deleteExtraSpaces(regexString);

    vector<string>splitedString;
    for (int i = 0; i < regexString.size(); i++)
    {
        /* epsilon */
        if (i != 0 && regexString[i] == 'L' && regexString[i-1] == '\\')
        {
            char c = ' ';
            string t = "";
            t += c;
            splitedString.push_back(t);
            continue;
        }
        /* escape charachter */
        if (i != 0 && regexString[i] == '\\' && regexString[i-1] == '\\')
        {
            splitedString.push_back("\\");

            continue;
        }

        if (regexString[i] == ' ' || regexString[i] == '\\')
            continue;


        /* operations of the regex string without scape charachter before it  (, ), *, |, ., +  */
        if ((i == 0 || regexString[i-1] != '\\') && (regexString[i] == '(' || regexString[i] == ')' ||
                regexString[i] == '*' || regexString[i] == '|' || regexString[i] == '+' ) )
        {
            string charToString = getOppName(regexString[i]);
            splitedString.push_back(charToString);

        }
        else
        {
            string pushedString = "";
            pushedString = pushedString + regexString[i];
            i++;
            while(i < regexString.size() && !(regexString[i] == '(' || regexString[i] == ')' || regexString[i] == ' ' || regexString[i] == '\\' ||
                                              regexString[i] == '*' || regexString[i] == '|' || regexString[i] == '+') )
            {

                if(i > 0&& regexString[i - 1] == '\\' && regexString[i] == '\\')
                {
                    pushedString += '\\';
                }
                if (regexString[i] == '\\')
                {
                    i++;
                    continue;
                }
                pushedString += regexString[i];
                i++;

            }
            i--;
            trim(pushedString);
            splitedString.push_back(pushedString);
        }
    }


    recognizeDashOpp(splitedString);
    return splitedString;

}


void RegularParser::recognizeDashOpp(vector<string>& strVec)
{
    if (strVec.size() < 3)
        return; /* as it does not contain dash operation */
    vector<string>vec;
    vec.push_back(strVec[0]);
    for (int i = 1; i + 1 < strVec.size(); i++)
    {
        if (strVec[i] == "-" &&isDashOpp(strVec[i - 1], strVec[i + 1]))
        {
            vec.pop_back(); /* as this string is one of the dash operation */
            buildRange(vec, strVec[i - 1], strVec[i + 1]);
            i++; /* to drop next string */
        }
        else
        {
            vec.push_back(strVec[i]);
        }
        if (i + 2 == strVec.size())
        {
            vec.push_back(strVec[strVec.size() - 1]);
        }
    }
    strVec = vec;
}

bool RegularParser::isDashOpp(string str1, string str2)
{
    /* check if it is number range like 12 - 30 or 0 - 9 */
    if (isNumber(str1) && isNumber(str2))
    {
        int n1 = stoi(str1);
        int n2 = stoi(str2);
        if (n1 < n2)
        {
            return true;
        }
        else
        {
            return false;
        }

    }
    /* then it may be charachter range like a - c or A- Z */
    if (str1.size() == 1 && str2.size() == 1 && str1[0] < str2[0])
    {
        if (isSmallChar(str1[0]) && isSmallChar(str2[0]))
        {
            return true;
        }
        else if (isCapitalChar(str1[0]) && isCapitalChar(str2[0]))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

void RegularParser::buildRange(vector<string>& vec, string str1, string str2)
{
    /* add brackets to make sure that it does not conflict with other operations. */
    vec.push_back("open op");
    if (isNumber(str1))
    {
        int n1 = stoi(str1);
        int n2 = stoi(str2);
        for (int i = n1; i <= n2; i++)
        {
            vec.push_back(to_string(i));
            if (i != n2)
            {
                vec.push_back("union op");
            }
        }
    }
    else
    {
        char c1 = str1[0];
        char c2 = str2[0];
        for (char c = c1; c <= c2; c++)
        {
            string s = "";
            s = s + c;
            vec.push_back(s);
            if (c != c2)
            {
                vec.push_back("union op");
            }
        }
    }
    vec.push_back("close op");
}


string RegularParser::getOppName(char c)
{

    switch(c)
    {
    case '*':
        return "closure op";
    case '+':
        return "positive closure op";
    case '(':
        return "open op";
    case ')':
        return "close op";
    case '|':
        return "union op";
    case '.':
        return "concatenate op";
    default:
        return "ERROR"; /*  this must not happened as it calls only in the previous cases */
    }

}
bool RegularParser::isOp(string s)
{
    if(s == "closure op" || s == "positive closure op" || s == "lookahead op" ||
            s == "open op" || s == "close op" || s == "union op" || s == "concatenate op"  )
    {

        return true;
    }
    return false;
}



vector<DataEntry> RegularParser::prepareNFA(vector<string> splitedString)
{
    vector<DataEntry>vec;
    DataEntry start;
    start.isOp = true;
    start.name = "open op";
    vec.push_back(start);
    for (int i = 0; i < splitedString.size(); i++)
    {
        DataEntry x;
        x.name = splitedString[i];
        if(isOp(splitedString[i]))
        {
            x.isOp = true;
        }
        else
        {
            if (regDefinition.find(x.name) != regDefinition.end())
            {
                x.nfa = regDefinition[x.name];
            }
            else
            {
                x.nfa = NFA(x.name[0]);
                for (int j = 1; j < x.name.size(); j++)
                {
                    NFA y(x.name[j]);
                    x.nfa.concatenate(y);
                }
            }
        }
        vec.push_back(x);
    }
    DataEntry endd;
    endd.isOp = true;
    endd.name = "close op";
    vec.push_back(endd);
    return vec;
}

DataEntry RegularParser::calculateRegexWithPriority(vector<DataEntry>vec)
{
    stack<DataEntry>stackk;
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i].isOp && vec[i].name == "close op")   /* that means the operation is open bracket ( */
        {
            vector<DataEntry>exp;
            while(stackk.size() && !(stackk.top().isOp && stackk.top().name == "open op") )
            {
                exp.push_back(stackk.top());
                stackk.pop();
            }
            if (stackk.size() == 0)   /*   there is an error in the expression*/
            {
                DataEntry res;
                res.haveError = true;

                return res;
            }
            else
            {
                stackk.pop();
                reverse(exp.begin(), exp.end());
                DataEntry res = calcExpression(exp);
                if (res.haveError)
                {
                    DataEntry res;

                    res.haveError = true;
                    return res;
                }
                stackk.push(res);
            }
        }
        else
        {
            stackk.push(vec[i]);
        }
    }
    return stackk.top();
}

DataEntry RegularParser::calcExpression(vector<DataEntry>exp)
{
    /* positive closure */
    DataEntry error;
    error.haveError = true;
    /* start by closure and positive closure */
    vector<DataEntry>exp2;
    if (exp.size() == 0)
    {
        return error; /* as the expresion contain () which is not allowed */
    }
    for (int i = 0; i < exp.size(); i++)
    {
        if (exp[i].isOp && exp[i].name == "closure op")
        {
            if (i == 0 || exp[i-1].isOp)
            {
                return error;
            }
            exp2[exp2.size() - 1].nfa.closure();
        }
        else if (exp[i].isOp && exp[i].name == "positive closure op")
        {

            if (i == 0 || exp[i-1].isOp)
            {
                return error;
            }
            exp2[exp2.size() - 1].nfa.positiveClosure();


        }
        else
        {
            exp2.push_back(exp[i]);
        }
    }

    exp = exp2;
    exp2.clear();

    /* make concatenation */
    for (int i = 0; i < exp.size(); i++)
    {
        if (exp[i].isOp && exp[i].name == "concatenate op")
        {
            if (i == 0 || exp[i-1].isOp || i + 1 == exp.size() || exp[i + 1].isOp)
            {
                return error;
            }
            exp2[exp2.size() - 1].nfa.concatenate(exp[i + 1].nfa);
            i++; /* escape next token as it is already concatenated */
        }
        else if (!exp[i].isOp && i > 0 && !exp[i-1].isOp)
        {

            if (i == 0 || exp[i-1].isOp)
            {
                return error;
            }
            exp2[exp2.size() - 1].nfa.concatenate(exp[i].nfa);


        }
        else
        {
            exp2.push_back(exp[i]);
        }
    }


    exp = exp2;
    exp2.clear();
    /* make union */
    for (int i = 0; i < exp.size(); i++)
    {
        if (exp[i].isOp && exp[i].name == "union op")
        {
            if (i == 0 || exp[i-1].isOp || i + 1 == exp.size() || exp[i + 1].isOp)
            {

                return error;
            }
            exp2[exp2.size() - 1].nfa.unionn(exp[i + 1].nfa);
            i++; /* escape next token as it is already concatenated */
        }
        else
        {
            exp2.push_back(exp[i]);
        }
    }
    exp = exp2;



    if(exp.size() == 1 && !exp[0].isOp)
        return exp[0];
    return error;
}

DataEntry RegularParser::getNFA(string regexString)
{
    regexString = '(' + regexString + ')';
    vector<string>splitedString = split(regexString); /* split string by regex elements */
    vector<DataEntry>vec = prepareNFA(splitedString);
    DataEntry finalExpression = calculateRegexWithPriority(vec);
    return finalExpression;
}

void RegularParser::addStringToNFA(string s)
{
    NFA x(s[0]);
    for (int i = 1; i < s.size(); i++)
    {
        NFA y(s[i]);
        x.concatenate(y);
    }
    x.priority = priority;
    priority = priority + 1;
    x.name = s;
    nfaVector.push_back(x);
}


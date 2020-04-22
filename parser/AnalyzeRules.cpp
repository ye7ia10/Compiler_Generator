#include "AnalyzeRules.h"
#include "Rules.h"
#include "../lexical/include/readFile.h"
#include "../lexical/include/stringParsing.h"
AnalyzeRules::AnalyzeRules(string fileName, Rules* rules)
{
    buildRules(fileName, rules);
    //rules->removeLeftFactoring();
}

AnalyzeRules::~AnalyzeRules()
{
    //dtor
}
void AnalyzeRules::buildRules(string fileName, Rules* rules) {

    vector<string> lines = readFile(fileName);
    lines = splitRules(lines);
    bool firstRule = true;
    for (string s : lines) {
        vector<string> splited = splitByEgual(s);
        if (splited.size() != 2) {
            /* an error found as the rule should have exactly one equal '=' */
        } else {
            Rule* rule = new Rule(splited[0], splited[1]);
             rule->setFirstRule(firstRule);
            if (firstRule){
             rules->setFirstRule(splited[0]);
            }
            firstRule = false;
            cout << "parsing: " << rule ->getName() << "->" << splited[1] <<endl;
            rules->addRule(rule);
        }
    }

}


vector<string> AnalyzeRules::splitRules(vector<string> lines) {
    vector<string>v;
    string res = "";
    if (lines.size() == 0) return v;
    for (int i = 0; i < lines.size(); i++) {
        trim(lines[i]);
        if (lines[i].size()) {
            if (res.size()) {
                if (lines[i][0] == '#') {
                    if (res[0] == '#') {
                        string s = res.substr(1, res.size() - 1);
                        trim(s);
                        if (s.size())
                            v.push_back(s);
                    } else {
                        /* an error found as the rule does not start with # */

                    }
                    res = lines[i];
                } else {
                    res += " " + lines[i];
                }
            } else {
                res = lines[i];
            }
        }
    }
    if (res.size()) {
        if (res[0] == '#') {
            string s = res.substr(1, res.size() - 1);
            trim(s);
            if (s.size())
                v.push_back(s);
        } else {
            /* an error found as the rule does not start with # */

        }
    }
    return v;
}

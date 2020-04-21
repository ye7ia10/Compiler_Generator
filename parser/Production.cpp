#include "Production.h"

Production::Production(string s)
{
    this->name = s;
    buildProduction(s);
}

Production::~Production()
{
    //dtor
}


void Production::buildProduction(string s)
{
    vector<string> splited = splitByChar(s, ' ');
    for (string elm : splited)
    {
        if (elm != " ")
        {
            RuleComponent* component = new RuleComponent(elm);
            this->elements.push_back(component);
        }

    }
}

RuleComponent*Production:: getRlueComponent(int i) {
    return elements[i];
}

void Production::addFirst(vector<RuleComponent*> first) {
    removeDubLicates(first);
    this->first = first;
}


void Production::toString() {
    cout << "Production name: " << name << endl;
    cout << "First: ";
    for (RuleComponent* c : first) {
        cout << c->getName() << " ";
    }
    cout << endl;
}
void Production::removeDubLicates(vector<RuleComponent*>& v) {
    vector<RuleComponent*> c;
    for (int i = 0; i < v.size(); i++) {
        bool found = false;
        for (int j = 0; j < c.size(); j++) {
            if(v[i]->getName() == c[j]->getName()) {
                found = true;
                break;
            }
        }
        if (!found) {
            c.push_back(v[i]);
        }
    }
    v = c;
}
vector<RuleComponent*>Production::getFirst(){
    return first;
}
string Production::getName(){
    return name;
}

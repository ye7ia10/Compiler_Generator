#include "stringParsing.h"

void trim(string& str) {
    int first = 0, last = str.size() - 1;

    while(first < str.size() && str[first] == ' ') first++;

    while(last >= first && str[last] == ' ') last--;
    if (last < first) {
        str = "";
        return;
    }
    str = str.substr(first, (last - first + 1));
}

void deleteExtraSpaces(string& str) {
    trim(str);
    string s = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ' ' && (i && s[s.size() - 1] == ' ')) continue;
        s += str[i];
    }
    trim(s);
    str = s;
}

void addNeededSpace(string& str, char c) {
    string s = "";
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == c) {
            s = s + ' ' + str[i] + ' ';
        } else {
            s = s + str[i];
        }
    }
    str = s;
}

bool isSmallChar(char c) {


    if (c >= 'a' && c <= 'z') return true;

    return false;

}

bool isCapitalChar(char c) {

    if (c >= 'A' && c <= 'Z') return true;

    return false;
}

bool isNumber(string s) {
    if (s.size() == 0) return false;
    for (int i = 0; i < s.size(); i++) {
        if (!isdigit(s[i])) return false;
    }
    return true;
}

string addNumber(string s, int x) {
    string ans = "";
    string str = "";
    if(s.size() == 0) return ""; /* should not be done as there is no number */
    for (int i = 0; i < s.size(); i++) {
        if(s[i] == ' ')continue; /* if space found then ignore it */
        if (s[i] == ',') {
            /* separator found */
            int number = stoi(str);
            number = number + x;
            ans = ans + to_string(number) + ',';
            str = "";
        }else {
            str += s[i];
        }
        if (i + 1 == s.size()) {
            /* last number in s */
            int number = stoi(str);
            number = number + x;
            ans = ans + to_string(number);
            str = "";
        }
    }
    return ans;
}

vector<string>splitByChar(string s, char c) {
    vector<string>v;
    trim(s);
    deleteExtraSpaces(s);
    string str = "";
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == c) {
            v.push_back(str);
            str = "";
        } else {
            str += s[i];
        }
    }
    if (str.size() > 0) {
        v.push_back(str);
    }
    return v;
}

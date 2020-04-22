#ifndef STRINGPARSING_H_INCLUDED
#define STRINGPARSING_H_INCLUDED
#include <bits/stdc++.h>
using namespace std;
/**
this file contains some string functions that we use during the project
**/

/* str -> trim to delete spaces at the beginning and at the ending of the string */
void trim(string& str);

/* str -> string to be trimed and any extra spaces replaced by one space */
void deleteExtraSpaces(string& str);


/* separate charachter c in the string str by adding spaces before and after c */
void addNeededSpace(string& str, char c);

/* check if c is small English charachter */
bool isSmallChar(char c);

/* check if c is capital English charachter */
bool isCapitalChar(char c);

/* check if string s represent natural number */
bool isNumber(string s);

/* add number x to every number in string s.
numbers in s seperated by (,)
also be care that this function assume that the input is formated in this form
*/
string addNumber(string s, int x);

/* split string s to vector of strings by separator charachter c */
vector<string>splitByChar(string s, char c);
/*split string s to vector of strings by separator for first = only*/
vector<string>splitByEgual(string s);


#endif // STRINGPARSING_H_INCLUDED

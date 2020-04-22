#include "ParsedLine.h"

ParsedLine::ParsedLine(string inputLine, vector<string> parsed, int line, bool hasError)
{
    this->inputLine = inputLine;
    this->line = line;
    this->parsed = parsed;
    this->hasError = hasError;
}

ParsedLine::~ParsedLine()
{
    //dtor
}

string ParsedLine::getInputLine() {
    return inputLine;
}

vector<string> ParsedLine::getParsedLine() {
    return parsed;
}

int ParsedLine::getLine() {
    return line;
}

bool ParsedLine::isError() {
    return hasError;
}

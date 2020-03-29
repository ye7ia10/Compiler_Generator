#include "../header/readFile.h"

vector<string>readFile(string fileName)
{
    ifstream infile(fileName);
    vector<string> lines;
    string line;

    while (getline(infile, line))
    {
        istringstream iss(line);
        if (!iss)
        {
            return lines;
        }
        lines.push_back(line);
    }
    return lines;
}

#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <iterator>
#include <queue>
#include <stack>
#include <sstream>
#include <locale>
#include <stdlib.h>
#include <math.h>

using namespace std;

class Expression
{
public:
    Expression(const string& input);
    bool Evaluate(const vector<string>& rpn, string& result);
    bool InfixToRPN(vector<string>& inputs);
    void ReplaceAll(string& str, const string& from, const string& to);
    void Tokenize(list<string>& tokens, const string& delimiter);
    string inString;
    double ans;
};

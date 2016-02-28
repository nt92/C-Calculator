//
//  main.cpp
//  Calculator
//
//  Created by Nikhil Thota on 2/28/16.
//  Copyright © 2016 Nikhil Thota. All rights reserved.
//

#include <iostream>
#include <vector>
#include <iterator>
#include <queue>
#include <stack>
#include <list>
using namespace std;

const string charSet[] = { "(", ")", "+", "-", "*", "/", "^" };

int operatorPrecedence(const string& s)
{
    if(s == "^")
        return 3;
    
    else if(s == "*" || s == "/")
        return 2;
    
    else if(s == "+" || s == "-")
        return 1;
    
    return 0;
}

bool isOperator(const string& s)
{
    return s == "+" || s == "-" || s == "/" || s == "*" || s == "^";
}

void replaceAll(string& str, const string& from, const string& to)
{
    size_t start_pos = 0;
    
    while((start_pos = str.find(from, start_pos)) != string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

void tokenize(string& in, list<string>& tokens, const string& delimiter)
{
    size_t size = sizeof(charSet) / sizeof(string);
    
    for(int i = 0; i < static_cast<int>(size); i++)
    {
        string s = charSet[i];
        replaceAll(in, s, " " + s + " ");
    }
}

void infixToPostfix(string *in, vector<string>& out)
{
    
}

int main()
{
    string infix = "1 + 1";
    
    vector<string> postfix;
    
    infixToPostfix(&infix, postfix);
    
    cout << "HEY!" << endl;
    return 0;
}

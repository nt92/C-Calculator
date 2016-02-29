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

int OperatorPrecedence(const string& s)
{
    if(s == "^")
        return 3;
    
    else if(s == "*" || s == "/")
        return 2;
    
    else if(s == "+" || s == "-")
        return 1;
    
    return 0;
}

bool LeftAssociative(const string& s)
{
    if(s == "*" || s == "/" || s == "+" || s == "-")
        return true;
    
    return false;
}

bool IsOperator(const string& s)
{
    return s == "+" || s == "-" || s == "/" || s == "*" || s == "^";
}

bool IsNumber(const string& s)
{
    string::const_iterator it = s.begin();
    while(it != s.end() && isdigit(*it, locale()))
        ++it;
    
    return !s.empty() && it == s.end();
}

bool IsVariable(const string& s)
{
    if(s != "quit" && s != "let" && !IsOperator(s) && !IsNumber(s))
        return true;
    
    return false;
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

void Tokenize(string& in, list<string>& tokens, const string& delimiter)
{
    size_t size = sizeof(charSet) / sizeof(string);
    
    for(int i = 0; i < static_cast<int>(size); i++)
    {
        string s = charSet[i];
        replaceAll(in, s, " " + s + " ");
    }
    
    size_t next_pos = 0;
    size_t init_pos = in.find_first_not_of(delimiter, next_pos);
    
    while(next_pos != string::npos &&
          init_pos != string::npos)
    {
        // Get next delimiter position
        next_pos = in.find(delimiter, init_pos);
        
        string token = in.substr(init_pos, next_pos - init_pos);
        tokens.push_back(token);
        
        init_pos = in.find_first_not_of(delimiter, next_pos);
    }
    
    string firstToken = tokens.front();
    if(firstToken == "-")
    {
        list<string>::iterator it = tokens.begin();
        it++;
        
        if(it == tokens.end())
        {
            return;
        }
        
        string nextToken = *(it);
        
        if(IsNumber(nextToken))
        {
            tokens.pop_front();
            tokens.front() = firstToken + nextToken;
        }
        else if(nextToken == "(")
        {
            tokens.front() = firstToken + "1";
            tokens.insert(it, "*");
        }
        
        else if(nextToken == "-" && firstToken == "-")
        {
            list<string>::iterator nit = it;
            advance(nit, -1);
            tokens.erase(it);
            tokens.erase(nit);
        }
    }
    
    typedef list<string>::iterator t_iter;
    string prevToken = "";
    for(t_iter it = tokens.begin(); it != prev(tokens.end()); it++)
    {
        string token = *it;
        
        list<string>::iterator nit = it;
        advance(nit, 1);
        
        if(nit == tokens.end())
            break;
        
        string ntoken = *nit;
        
        if(token == "-" && prevToken == "(")
        {
            if(IsNumber(ntoken))
            {
                tokens.erase(nit);
                *it = "-" + ntoken;
                token = *it;
            }
        }
        
        else if(token == "-" && (IsOperator(prevToken) || prevToken == "^" || prevToken == "%"))
        {
            if(token == "-" && prevToken == "-")
            {
                list<string>::iterator nit = it;
                list<string>::iterator nnit = nit;
                nnit++;
                advance(nit, -1);
                tokens.erase(it);
                *nit = "+";
                
                list<string>::iterator pnit = nit;
                advance(pnit, -1);
                
                if(IsOperator(*pnit) || *pnit == "(")
                    tokens.erase(nit);
                
                token = *nnit;
                it = nnit;
                
                if(it == prev(tokens.end()))
                    break;
                
            }
            else if(IsNumber(ntoken))
            {
                bool exit = false;
                if(nit == prev(tokens.end()))
                    exit = true;
                
                tokens.erase(nit);
                *it = "-" + ntoken;
                token = *it;
                
                if(exit)
                    break;
            }
            else if(ntoken == "(")
            {
                *it = "-1";
                token = *it;
                tokens.insert(nit, "*");
            }
        }
        
        prevToken = token;
    }
    
    prevToken = "";
    t_iter prevIt;
    
    for(t_iter it = tokens.begin(); it != tokens.end(); it++)
    {
        string token = *it;
        
        if(token == "(" && prevToken == "-")
        {
            tokens.insert(it, "1");
            tokens.insert(it, "*");
        }
        
        prevToken = token;
        prevIt = it;
    }
}

bool infixToPostfix(string& in, vector<string>& out)
{
    list<string> infix;
    Tokenize(in, infix, " ");
    
    typedef list<string>::const_iterator tok_iter;
    stack<string> stack;
    queue<string> outputQueue;
    
    bool success = true;
    
    for(tok_iter it = infix.begin(); it != infix.end(); it++)
    {
        string token = *it;
        
        if(IsNumber(token) || IsVariable(token))
            outputQueue.push(token);
        
        else if(IsOperator(token))
        {
            while(!stack.empty() && IsOperator(stack.top()) && (OperatorPrecedence(token) <= OperatorPrecedence(stack.top()) || (LeftAssociative(token) && OperatorPrecedence(token) <= OperatorPrecedence(stack.top()))))
            {
                outputQueue.push(stack.top());
                stack.pop();
            }
            stack.push(token);
        }
        
        else if(token == "(")
        {
            stack.push(token);
        }
        
        else if(token == ")")
        {
            while(!stack.empty() && stack.top() != "(")
            {
                outputQueue.push(stack.top());
                stack.pop();
            }
            
            if(!stack.empty())
            {
                string stackToken = stack.top();
                if(stackToken != "(")
                    success = false;
            }
            else
                return false;
            
            stack.pop();
        }
    }
    
    while(!stack.empty())
    {
        if(stack.top() == "(" || stack.top() == ")")
            return false;
        
        outputQueue.push(stack.top());
        stack.pop();
    }
    
    while(!outputQueue.empty())
    {
        string addToken = outputQueue.front();
        out.push_back(addToken);
        outputQueue.pop();
    }
    
    return success;
}

int main()
{
    string infix = "1+1*ans";
    
    vector<string> postfix;
    
    infixToPostfix(infix, postfix);
    
    for(int i = 0; i < postfix.size(); i++)
        cout << postfix[i] << " ";
    
    return 0;
}

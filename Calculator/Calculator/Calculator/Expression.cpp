#include "Expression.hpp"

//charset to define which characters will be used for operators as well as parenthesis
const string charSet[] = { "(", ")", "+", "-", "*", "/", "^" };

Expression::Expression(const string& input)
{
    inString = input;
}

int OpArgCount(const string& s)
{
    //returns the number of arguments needed for a specific operation
    if(s == "+" || s == "-" || s == "*" || s == "/" || s == "^")
        return 2;
    
    return 1;
}

int OperatorPrecedence(const string& s)
{
    //basic precedence. ^ > * or / > + or -. else it's a var/number and it's 0
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
    //Is left associative if the token is one of these operators
    if(s == "*" || s == "/" || s == "+" || s == "-")
        return true;
    
    return false;
}

bool IsOperator(const string& s)
{
    //returns true if the token is one of these operators
    return s == "+" || s == "-" || s == "/" || s == "*" || s == "^";
}

bool IsNumber(const string& s)
{
    //Old code to check if number was an integer
//    string::const_iterator it = s.begin();
//    while(it != s.end() && isdigit(*it, locale()))
//    {
//        ++it;
//    }
//    
//    return !s.empty() && it == s.end();
    
    //new code to accommdate doubles. Checks if string has any only the given characters and if so, it's a number. integer or double
    return (strspn( s.c_str(), ".0123456789" ) == s.size());
}

bool IsVariable(const string& s)
{
    //if the token is not equal to a hardcoded number: quit or let, if it's not an operator or a number, and if it's not a parenthesis, it is a variable by definition.
    if(s != "quit" && s != "let" && !IsOperator(s) && !IsNumber(s) && s != "(" && s != ")")
        return true;
    
    return false;
}

void Expression::Tokenize(list<string>& tokens, const string& delimiter)
{
    size_t size = sizeof(charSet) / sizeof(string);
    
    for(int i = 0; i < static_cast<int>(size); i++)
    {
        string s = charSet[i];
        ReplaceAll(inString, s, " " + s + " ");
    }
    
    size_t next_pos = 0;
    size_t init_pos = inString.find_first_not_of(delimiter, next_pos);
    
    while(next_pos != string::npos &&
          init_pos != string::npos)
    {
        // Get next delimiter position
        next_pos = inString.find(delimiter, init_pos);
        
        string token = inString.substr(init_pos, next_pos - init_pos);
        tokens.push_back(token);
        
        init_pos = inString.find_first_not_of(delimiter, next_pos);
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
        
        else if(token == "-" && (IsOperator(prevToken) || prevToken == "^"))
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

void Expression::ReplaceAll(string& str, const string& from, const string& to)
{
    size_t start_pos = 0;
    
    while((start_pos = str.find(from, start_pos)) != string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

bool Expression::Evaluate(const vector<string>& rpn, string& result)
{
    typedef vector<string>::const_iterator rpn_iter;
    stack<string> stack;
    
    for(rpn_iter it = rpn.begin(); it != rpn.end(); it++)
    {
        string token = *it;
        
        if(IsVariable(token))
        {
            string temp = hash.get(token);
            if(temp != "NA")
            {
                token = temp;
            }
            else
            {
                cout << "Undeclared-Variable" << endl;
                return false;
            }
        }
        
        if(IsNumber(token))
        {
            stack.push(token);
        }
        
        else if(IsOperator(token))
        {
            double result = 0.0;
            
            unsigned int numArgs = OpArgCount(token);
            
            bool isUnary = false;
            unsigned long stackArgs = stack.size();
            vector<double> args;
            
            if(stackArgs < numArgs)
            {
                if(stackArgs == 1 && numArgs == 2 &&(token == "+" || token == "-"))
                {
                    string value = stack.top();
                    result = strtod(value.c_str(), NULL);
                    stack.pop();
                    isUnary = true;
                }
                else
                    return false;
                
            }
            else
            {
                while(numArgs > 0)
                {
                    string value = stack.top();
                    double d = strtod(value.c_str(), NULL);
                    args.push_back(d);
                    stack.pop();
                    numArgs--;
                }
            }
            
            if(IsOperator(token) && !isUnary)
            {
                double d2 = args[0];
                double d1 = args[1];
                
                if(token == "+")
                    result = d1 + d2;
                
                else if(token == "-")
                    result = d1 - d2;
                
                else if(token == "*")
                    result = d1 * d2;
                
                else if(token == "/")
                {
                    if(d2 == 0)
                    {
                        cout << "Divide-By-Zero" << endl;
                        return false;
                    }
                    result = d1 / d2;
                }
                
                else if(token == "^")
                {
                    double d2 = args[0];
                    double d1 = args[1];
                    result = pow((double) d1, d2);
                }
            }
            
            if(result ==(long)result)
                result = long(result);
            
            stringstream s;
            s << result;
            stack.push(s.str());
        }
    }
    
    if(stack.size() == 1)
    {
        result = stack.top();
        
        double res = strtod(result.c_str(), NULL);
        if(res ==(long) res)
        {
            long lres =(long) res;
            stringstream s;
            s << lres;
            result = s.str();
        }
        return true;
    }
    return false;
}

bool Expression::InfixToRPN(vector<string>& inputs)
{
    list<string> infix;
    Tokenize(infix, " ");
    
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
            while(!stack.empty() && IsOperator(stack.top()) &&
                  ((LeftAssociative(token) && OperatorPrecedence(token) == OperatorPrecedence(stack.top())) ||
                   (OperatorPrecedence(token) < OperatorPrecedence(stack.top()))))
            {
                outputQueue.push(stack.top());
                stack.pop();
            }
            
            stack.push(token);
            
        }
        else if(token == "(")
            stack.push(token);
        
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
        outputQueue.push(stack.top());
        stack.pop();
    }
    
    while(!outputQueue.empty())
    {
        string token = outputQueue.front();
        inputs.push_back(token);
        outputQueue.pop();
    }
    
    return success;
}

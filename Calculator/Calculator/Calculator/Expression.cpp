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
    return s == "+" || s == "-" || s == "/" || s == "*" || s == "^" || s == "=";
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
    for(int i = 0; i < inString.size(); i++)
    {
        string temp = inString.substr(i, 1);
        string token = "";
        
        if(IsOperator(temp) || temp == "(" || temp == ")")
        {
            token = temp;
        }
        
        else
        {
            while(!IsOperator(inString.substr(i, 1)) && i < inString.size() && inString.substr(i, 1) != "(" && inString.substr(i, 1) != ")")
            {
                token += inString.substr(i, 1);
                i++;
            }
            i--;
        }
        
        tokens.push_back(token);
    }
}

bool Expression::Evaluate(const vector<string>& rpn, string& result)
{
    typedef vector<string>::const_iterator rpn_iter;
    stack<string> stack;
    
    //iterate through the vector
    for(rpn_iter it = rpn.begin(); it != rpn.end(); it++)
    {
        string token = *it;
        
        //variable operation
        if(IsVariable(token))
        {
            //check if variable exists in hashmap, if not throw an error
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
        
        //if the token is a number, push onto stack for evaluation
        if(IsNumber(token))
        {
            stack.push(token);
        }
        
        //if the token is an operator
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
            
            //calculate value of expression
            if(IsOperator(token) && !isUnary)
            {
                double d2 = args[0];
                double d1 = args[1];
                
                //plus
                if(token == "+")
                    result = d1 + d2;
                
                //minus
                else if(token == "-")
                    result = d1 - d2;
                
                //multiply
                else if(token == "*")
                    result = d1 * d2;
                
                //divide
                else if(token == "/")
                {
                    //return error if divide by 0
                    if(d2 == 0)
                    {
                        cout << "Divide-By-Zero" << endl;
                        return false;
                    }
                    result = d1 / d2;
                }
                
                //exponentiation
                else if(token == "^")
                {
                    double d2 = args[0];
                    double d1 = args[1];
                    result = pow((double) d1, d2);
                }
            }
            
            //catch for returning long in case of integer/long operations
            if(result ==(long)result)
                result = long(result);
            
            //push string value onto the stack
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
    
    //declare stack, queue, and iterator for token list
    typedef list<string>::const_iterator tok_iter;
    stack<string> stack;
    queue<string> outputQueue;
    
    bool success = true;
    
    //iterates through infix list
    for(tok_iter it = infix.begin(); it != infix.end(); it++)
    {
        string token = *it;
        
        //if the token is a number or a variable push it onto the queue
        if(IsNumber(token) || IsVariable(token))
            outputQueue.push(token);
        
        else if(IsOperator(token))
        {
            //if the token is an operator, push it onto the stack provided the stack is not empty
            //while there is an operator2 at the top of the stack and either the first operator1 is left-associative and its precedence <= operator 2 or operator1 is not left-associative and has precedence < operator 2, pop operator2 off the stack and onto the output queue
            while(!stack.empty() && IsOperator(stack.top()) &&
                  ((LeftAssociative(token) && OperatorPrecedence(token) == OperatorPrecedence(stack.top())) ||
                   (OperatorPrecedence(token) < OperatorPrecedence(stack.top()))))
            {
                outputQueue.push(stack.top());
                stack.pop();
            }
            
            stack.push(token);
            
        }
        
        //if token is a left parenthesis, pus it onto the stack
        else if(token == "(")
            stack.push(token);
        
        //if the token is a right parenthesis, until the token at the top is left, pop operators off the stack onto the output queue. also pop the left parenthesis from the stack but not onto the output queue. if the token at the top of the stack is an operator, pop onto the output queue
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
                
                
                //if parenthesis mismatched, return a value of false stating that the expression is invalid
                if(stackToken != "(")
                    success = false;
            }
            else
                return false;
            
            stack.pop();
        }
    }
    
    
    //while still operator tokens in stack, pop onto the output queue
    while(!stack.empty())
    {
        outputQueue.push(stack.top());
        stack.pop();
    }
    
    //move all the values from the output queue onto the inputs list
    while(!outputQueue.empty())
    {
        string token = outputQueue.front();
        inputs.push_back(token);
        outputQueue.pop();
    }
    
    return success;
}

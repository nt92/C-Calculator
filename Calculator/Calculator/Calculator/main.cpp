#include "Expression.hpp"

using namespace std;

double string_to_double( const string& s )
{
    istringstream i(s);
    double x;
    if (!(i >> x))
        return 0;
    return x;
}

int main(int argc, char** argv)
{
    string s = "";
    Expression parser(s);
    
    while(s != "quit")
    {
        getline(cin, s);
            
        parser.inString = s;
        
        vector<string> postfix;
        
        if(s.substr(0,3) == "let")
        {
            parser.InfixToRPN(postfix);
            
            vector<string> postfixEval;
            
            for(int i = 2; i < postfix.size(); i++)
                postfixEval.push_back(postfix[i]);
            
            string str_result;
            parser.Evaluate(postfixEval, str_result);
            double result = string_to_double(str_result);
            
            parser.hash.put(postfix[0], result);
        }
            
        else if (parser.InfixToRPN(postfix))
        {
//            for(int i = 0; i < (int)postfix.size(); i++)
//            {
//                cout << postfix[i] << " ";
//            }
//            
            string str_result;
                
            if (parser.Evaluate(postfix, str_result))
            {
                double result = string_to_double(str_result);
                cout << result << endl;
            }
        }
        
    }
    return 0;
}

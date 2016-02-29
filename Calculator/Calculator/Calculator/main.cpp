// USE 'LOG' AND 'RT' CAPITALIZED TO WORK CORRECTLY
// PREV ITERATOR COMMANDS IN TOKENIZE METHOD ONLY WORK IN C++ 11 OR ABOVE

//Working Examples:
//[1]: 2 + 3 * 4 - 5 ^ 2 = -11
//[2]: 2 ^ 3 + 4 * 5 - 2 = 26
//[3]: ( 2 + 3 ) * ( 4 - 5 ) ^ 2  = 5
//[4]: 8 ^ ( -4 / 3 ) * 4 = 1 / 4
//[5]: 108 LOG 3 = 4

//[6]: 3 RT 108 * 3 RT 16 = 12

//[7]: (e ^ 3) LOG e = 3

//[8]: (7 - 3) LOG (2 RT 2) = 4
//[9]: 2 ^ (5 / 3 + pi) / ((3 RT 4) * 2 ^ pi) = 2

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
    
    while(s != "quit")
    {
        getline(cin, s);
            
        Expression parser(s);
        
        vector<string> postfix;
            
        if (parser.InfixToRPN(postfix))
        {
            for(int i = 0; i < (int)postfix.size(); i++)
            {
                cout << postfix[i] << " ";
            }
                
            string str_result;
                
            if (parser.Evaluate(postfix, str_result))
            {
                double result = string_to_double(str_result);
                cout << "\n" << result;
                    
                cout << endl;
            }
        }
        
    }
    return 0;
}

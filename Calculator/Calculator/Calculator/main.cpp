#include "Expression.hpp"

using namespace std;

//Function to convert a string to a double through stringstreams
double string_to_double(const string& s)
{
    istringstream i(s);
    double x;
    if (!(i >> x))
        return 0;
    return x;
}

string trim(string& str)
{
    //returns a string that is a substring of the first character without a space to the end of the string lacking spaces
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}

int main(int argc, char** argv)
{
    //initializes s and creates an Expression object with the input string
    string s = "";
    Expression parser(s);
    
    //keeps asking for input until the input is quit
    while(s != "quit")
    {
        getline(cin, s);
        s = trim(s);
        bool flag = false;
        if (s.size() > 3)
        {
            flag = s.substr(3,1) == " ";
        }
        
        s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
        
        parser.inString = s;
        
        //vector to hold the postfix tokens for either variable declaration or calculation
        vector<string> postfix;
        
        //variable declaration
        if(s.substr(0,3) == "let" && flag)
        {
            size_t eqPos = s.find("=");
            string key = s.substr(3, eqPos - 3);
            parser.inString = s.substr(eqPos+1);
            
            //calculates value after = to add to variable
            parser.InfixToRPN(postfix);
            
            //adds tokens of function to eval vector
            
            string str_result;
            parser.Evaluate(postfix, str_result);
            double result = string_to_double(str_result);
            
            //add the variable and its value to the hashmap
            parser.hash.put(key, result);
        }
        
        //expression calculation
        else if (parser.InfixToRPN(postfix))
        {
            //            for(int i = 0; i < (int)postfix.size(); i++)
            //            {
            //                cout << postfix[i] << " ";
            //            }
            
            string str_result;
            
            //if possible to evaluate, will evaluate. otherwise throws an error from evaluate function
            if (parser.Evaluate(postfix, str_result))
            {
                double result = string_to_double(str_result);
                cout << result << endl;
            }
        }
        
    }
    return 0;
}
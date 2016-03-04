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
#include <unordered_map>
#include <map>

using namespace std;

//hashmap using unordered_map that is easier to use. employs basic get, put, and printmap method for testing
class HashMap
{
    unordered_map<string, double> hashMap;
    
public:
    //put method either adds to hashmap if not already existing, or updates if the corresponding key exists
    void put(string key, double value) {
        if (get(key) == "NA") {
            hashMap.insert({key,value});
        }
        else {
            hashMap.at(key) = value;
        }
    }
    
    string get(string key) {
        unordered_map<string,double>::const_iterator got = hashMap.find(key);
        if (got == hashMap.end()) {
            return "NA";
        }
        else {
            stringstream ss;
            ss << got->second;
            return ss.str();
        }
    }
    
    //uses hashmap iterator to iterate and output each key-value pair
    void printMap(){
        for(auto it = hashMap.begin(); it != hashMap.end(); ++it)
        {
            cout << " " << it -> first << " : " << it->second << endl;
        }
    }
};

//header declaration for Expression class
class Expression
{
public:
    Expression(const string& input);
    bool Evaluate(const vector<string>& rpn, string& result);
    bool InfixToRPN(vector<string>& inputs);
    void ReplaceAll(string& str, const string& from, const string& to);
    void Tokenize(list<string>& tokens, const string& delimiter);
    HashMap hash;
    string inString;
    double ans;
};

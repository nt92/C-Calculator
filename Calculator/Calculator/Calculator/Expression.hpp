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

class HashMap
{
    unordered_map<string, double> hashMap;
    
public:
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
    
    void printMap(){
        for(auto it = hashMap.begin(); it != hashMap.end(); ++it)
        {
            cout << " " << it -> first << ": " << it->second << endl;
        }
    }
};

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

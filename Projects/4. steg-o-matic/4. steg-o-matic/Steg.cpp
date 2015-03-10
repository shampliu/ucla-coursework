#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

bool Steg::hide(const string& hostIn, const string& msg, string& hostOut) 
{
    // empty string
    if (hostIn == "") {
        return false;
    }
    
    vector<unsigned short> numbers;
    Compressor::compress(msg, numbers);
    
    string code = BinaryConverter::encode(numbers);
    
    vector<string> lines;
    string line = "";
    
    for (int i = 0; i < hostIn.length(); i++) {
        if (hostIn[i] == '\n') {
            lines.push_back(line);
            line = "";
            continue;
        }
        else if (hostIn[i] == '\r') {
            i++;
            lines.push_back(line);
            line = "";
            continue;
        }
        else {
            line += hostIn[i];
        }
    }
    
    // line that didn't end in a newline
    if (line != "") {
        lines.push_back(line);
        line = "";
    }
    
    for(auto& str : lines) {
        // strip the trailing whitespace
        for (int i = str.length()-1; i >= 0; i--) {
            if (str[i] == ' ' || str[i] == '\t') {
                str = str.substr(0, str.size()-1);
            }
            else {
//                string b = str.substr(0, str.size()-1);
//                cout << b << endl;
                break;
            }
        }
    }
    
    int l = code.length(); // 128
    int n = lines.size(); // 5
    
    
    
    // to manage substring
    int count = 0;
    int index = 0;
    int inc = l/n; // 25
    
    for (int i = 0; i < n; i++) {
        // first l%n lines
        if (i < l%n) { // i < 3

            lines[i] += code.substr(count, inc + 1);
            count += inc + 1;
//            for (int j = 0; j < inc + 1; j++) {
//                lines[i] += code[index];
//                index++;
//            }
        }
        // the rest
        else {
//            for (int j = 0; j < inc; j++) {
//                lines[i] += code[index];
//                index++;
//            }

            lines[i] += code.substr(count, inc);
            count += inc;
        }
        
    }
    

    
    
    
    hostOut = "";
    for (const auto& str : lines) {
        hostOut = hostOut + str + '\n';
    }
    
	return true;  // This compiles, but may not be correct
}

bool Steg::reveal(const string& hostIn, string& msg) 
{
    vector<string> lines;
    string line = "";
    
    for (int i = 0; i < hostIn.length(); i++) {
        if (hostIn[i] == '\n') {
            lines.push_back(line);
            line = "";
            continue;
        }
        else if (hostIn[i] == '\r') {
            i++;
            lines.push_back(line);
            line = "";
            continue;
        }
        else {
            line += hostIn[i];
        }
    }
    
    string result = "";
    for (auto& str : lines) {
        // strip only trailing whitespace
        if (str.length() == 0) {
            continue;
        }
        if (str.length() == 1) {
            if (str[0] == ' ' || str[0] == '\t') {
                result += str[0];
            }
            continue; 
        }
        int i;
        int count = 0;
        
        for (i = str.length()-1 ; i >= 0; i--) {
            if (str[i] == ' ' || str[i] == '\t') {
                count++;
            }
            else break;
        }
        
        result += str.substr(i+1, count);
//        cout << result.length() << endl;
    }
    
    
    cout << result.length();
    
    vector<unsigned short> numbers;
    BinaryConverter::decode(result, numbers);
//    cout << numbers.size();
    Compressor::decompress(numbers, msg);
    
    
    
    return true;
}



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
        // strip only trailing whitespace
        for (int i = str.length()-1; i >= 0; i--) {
            if (str[i] == ' ' || str[i] == '\t') {
                str = str.substr(0, str.size()-1);
            }
            else break;
        }
    }
    
    int l = code.length();
    int n = lines.size();
    
    // to manage substring
    int count = 0;
    
    for (int i = 0; i < n; i++) {
        // first l%n lines
        if (i < l%n) {
            lines[i] += code.substr(count, count + (l/n) + 1);
            count += (l/n) + 1;
        }
        // the rest
        else {
            lines[i] += code.substr(count, count + (l/n));
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
//    vector<string> lines;
//    string line = "";
//    
//    for (int i = 0; i < hostIn.length(); i++) {
//        if (hostIn[i] == '\n') {
//            lines.push_back(line);
//            line = "";
//            continue;
//        }
//        else if (hostIn[i] == '\r') {
//            i++;
//            lines.push_back(line);
//            line = "";
//            continue;
//        }
//        else {
//            line += hostIn[i];
//        }
//    }
//    
//    string result = "";
//    for (auto& str : lines) {
//        // strip only trailing whitespace
//        if (str.length() == 0) {
//            continue;
//        }
//        if (str.length() == 1) {
//            if (str[0] == ' ' || str[0] == '\t') {
//                result += str[0];
//            }
//            continue; 
//        }
//        
//        int i = str.length()-1;
//        int count = 0;
//        
//        for ( ; i >= 0; i--) {
//            if (str[i] == ' ' || str[i] == '\t') {
//                count++;
//                continue;
//            }
//            else break;
//        }
//        std::cout << i << " --- " << str.length() << std::endl;
////        result += str.substr(i, count);
////        std::cout << str.length() << " ";
//    }
//    
//    vector<unsigned short> numbers;
//    BinaryConverter::decode(result, numbers);
////    Compressor::decompress(numbers, msg);
//    
//    
//    
    return true;
}



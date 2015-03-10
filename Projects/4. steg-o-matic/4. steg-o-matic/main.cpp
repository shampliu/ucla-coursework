// We have not yet produced the test driver main routine for you.

//#include "provided.h"
#include <iostream>
#include "provided.h"
#include <string>
//#include "HashTable.h"

using namespace std;

int main()
{
//    HashTable<string, unsigned int> hash(10, 20);
//    hash.set("hello", 69);
//
//    unsigned int result = 15;
//    hash.get("hello", result);
//
//    cout << result << endl;
//
//    hash.set("bye", 111);
//    hash.set("hi", 52);
//    
//    hash.touch("bye"); 
//
//    string k = "";
//    unsigned int i = 4;
//    
//    hash.discard(k, i);
//    cout << k << i;
//    
//    hash.set("cat", 115);
//    hash.get("cat", result);
//    cout << result;
    
    

//    vector<unsigned short> v;
//    v.push_back(15);
//    v.push_back(17);
//    v.push_back(66);
//    
//    string s = BinaryConverter::encode(v);
//    cout << s;
    
    string st = "<html>      \nQ  \tQQ  \t\nBBB\t\t\t     \n\nGG_\t\t\n";
    string msg = "AAAAAAAAAB";
    string out;
    Steg::hide(st, msg, out);
    
    cout << out;
    
    string blah = "";
    
    Steg::reveal(out, blah);
    
    cout << blah;
    
//    vector<unsigned short> numbers;
//    Compressor::compress(msg, numbers);
//    
//    string code = BinaryConverter::encode(numbers);
//    cout << code.length();
//    
//    BinaryConverter::decode(code, numbers);
//    Compressor::decompress(numbers, out);
//    cout << out;
    
//    cout << out.length();
//    
//    for (int i = 0; i < out.length(); i++) {
//        if (out[i] == '\t') {
//            cout << "T";
//            continue;
//        }
//        if (out[i] == ' ') {
//            cout << "=";
//            continue;
//        }
//        cout << out[i];
//    }
//    
    

	// string text;
	// if ( ! WebSteg::hideMessageInPage("http://cs.ucla.edu", "Hello there!", text))
	//	cout << "Error hiding!" << endl;
	// string msg;
	// if ( ! WebSteg::revealMessageInPage("http://cs.ucla.edu", msg))
	// 	cout << "Error revealing!" << endl;
}

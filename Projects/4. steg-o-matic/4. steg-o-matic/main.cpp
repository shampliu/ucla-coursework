// We have not yet produced the test driver main routine for you.

//#include "provided.h"
#include <iostream>
#include "HashTable.h"
using namespace std;

int main()
{
    HashTable<string, unsigned int> hash(10, 20);
    hash.set("hello", 69);
    
    unsigned int result = 15;
    hash.get("hello", result);
    
//    cout << result << endl;
    
    hash.set("bye", 111);
    hash.set("hi", 52);
    
    hash.touch("bye"); 
    
    string k = "";
    unsigned int i = 4;
    
    hash.discard(k, i);
    cout << k << i;
    
    
	cout << "Test driver not yet written." << endl;

	// string text;
	// if ( ! WebSteg::hideMessageInPage("http://cs.ucla.edu", "Hello there!", text))
	//	cout << "Error hiding!" << endl;
	// string msg;
	// if ( ! WebSteg::revealMessageInPage("http://cs.ucla.edu", msg))
	// 	cout << "Error revealing!" << endl;
}

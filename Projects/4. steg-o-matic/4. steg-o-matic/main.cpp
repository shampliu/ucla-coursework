// We have not yet produced the test driver main routine for you.

//#include "provided.h"
#include <iostream>
#include "provided.h"
#include <string>
#include "http.h"
//#include "HashTable.h"

using namespace std;

int main()
{
    
    /* REVIEW THIS LATER */
//    string st = "<html>      \nQ  \tQQ  \t\nBBB\t\t\t     \n\nGG_\t\t\n";
    string st = "Hi.\nBlah      \nBlah";
    string msg = "Hey Hey Hey H";
    string out;
    Steg::hide(st, msg, out);
    
    cout << endl;
    string blah = "";
    
    Steg::reveal(out, blah);
    cout << blah;
    
//    string text;
//    if ( ! WebSteg::hideMessageInPage("http://cs.ucla.edu", "Hello there!", text)) {
//        cout << "Error hiding!" << endl;
//    }
//    HTTP().set("http://a.com", text);
//    string msg;
//    if ( ! WebSteg::revealMessageInPage("http://a.com", msg) || msg != "Hello there!") {
//        cout << "Error revealing!" << endl;
//    }
//    
//    cout << msg;
    
    
}

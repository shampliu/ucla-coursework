
#include <iostream>
#include "provided.h"
#include <string>
#include "http.h"

using namespace std;

int main()
{
    
    /* REVIEW THIS LATER */
//    string st = "<html>      \nQ  \tQQ  \t\nBBB\t\t\t     \n\nGG_\t\t\n";
//    string st = "Hi.\nBlah      \nBlah";
//    string msg = "Hey Hey Hey H";
//    string out;
//    Steg::hide(st, msg, out);
//    
//    cout << endl;
//    string blah = "";
//    
//    Steg::reveal(out, blah);
//    cout << blah;
    
//    string text;
//    if ( ! WebSteg::hideMessageInPage("http://cs.ucla.edu", "Hello there! Hello there! Hello!", text)) {
//        cout << "Error hiding!" << endl;
//    }
//    HTTP().set("http://a.com", text);
//    string msg;
//    if ( ! WebSteg::revealMessageInPage("http://a.com", msg) || msg != "Hello there! Hello there! Hello!") {
//        cout << "Error revealing!" << endl;
//    }
//    
//    cout << msg;
    
    string originalPageText =
    "<html>\n"
    "<head>\n"
    " <title>My Pretend Web Page</title>\n"
    "</head>\n"
    "<body>\n"
    "<h2>My Pretend Web Page<h1>\n"
    "<p>\n"
    "I wish I were creative enough to have something interesting \n"
    "to say here.  I'm not, though, so this is a boring page.\n"
    "</p>\n"
    "<p>\n"
    "Oh, well.\n"
    "</p>\n"
    "</body>\n"
    "</html>\n"
    ;
    HTTP().set("http://boring.com", originalPageText);
    string plan =
    "Lefty and Mugsy enter the bank by the back door.\n"
    "Shorty waits out front in the getaway car.\n"
    "Don't let the teller trip the silent alarm.\n"
    ;
    string newPageText;
    if ( ! WebSteg::hideMessageInPage("http://boring.com", plan, newPageText))
    {
        cout << "Error hiding!" << endl;
        return 1;
    }
    HTTP().set("http://boring.com", newPageText);
    string msg;
    if ( ! WebSteg::revealMessageInPage("http://boring.com", msg))
    {
        cout << "Error revealing!" << endl;
        return 1;
    }
    if (msg != plan)
    {
        cout << "Message not recovered properly:\n" << msg;
        return 1;
    }
    cout << "Recovered text is what was hidden:\n" << msg;
    
    
    
    
}

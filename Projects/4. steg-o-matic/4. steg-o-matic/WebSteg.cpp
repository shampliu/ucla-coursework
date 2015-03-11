#include "provided.h"
#include <string>
#include "http.h"
#include <iostream>

using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
//    string url = "http://en.wikipedia.org/wiki/Bald";
//    string page; // to hold the contents of the web page
//    // The next line downloads a web page for you. So easy!
//    if (HTTP().get(url, page))
//        cout << page; // prints the page’s data out
//    else
//        cout << "Error fetching content from URL " << url << endl;
//    ...
    
//    HTTP().set("http://a.com", "This is a test page.");
//    HTTP().set("http://b.com", "Here is another.");
//    HTTP().set("http://c.com", "<html>Everyone loves CS 32</html>");
//    string page;
//    if (HTTP().get("http://b.com", page))
//        cout << page << endl; // writes Here is another.
    
    if (HTTP().get(url, host)) {
        string hostOut;
        Steg::hide(host, msg, hostOut);
        return true;
    }
    else {
        cout << "Error fetching content from URL " << url << endl;
        return false;
    }
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
    string hostIn;
    if (HTTP().get(url, hostIn)) {
        Steg::reveal(hostIn, msg);
        return true;
    }
    else {
        cout << "Error fetching content from URL " << url << endl;
        return false;
    }
}

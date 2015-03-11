#include "provided.h"
#include <string>
#include "http.h"
#include <iostream>

using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
    // get HTML at url and set it to host
    if (HTTP().get(url, host)) {
        string hostOut;
        Steg::hide(host, msg, hostOut);
        HTTP().set(url, hostOut);
        host = hostOut;
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

#include "provided.h"
#include <string>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
	return false;  // This compiles, but may not be correct
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
	return false;  // This compiles, but may not be correct
}

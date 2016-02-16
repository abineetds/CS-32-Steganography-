#include "provided.h"
#include "http.h"
#include <string>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
	string page;
	if (!HTTP().get(url, page))			//Get contents of URL in page
		return false;
	else
	{
		if (Steg::hide(page, msg, host))	//Hide messeage in those contents and get them back in host
			return true;
		else
			return false;
	}
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
	string page;
	if (!HTTP().get(url, page))			//Get contents of URL in page
		return false;
	else
	{
		if (Steg::reveal(page, msg))	//Gets hidden message in msg
			return true;	
		else
			return false;
	}
}

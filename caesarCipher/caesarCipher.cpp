#include <iostream>
using namespace std;

// 33 to 126 are printable characters

string encrypt(string s,int key)
{
	string ans=s;
	int val;
	for(int i=0;i<s.length();i++)
	{
		ans[i]=ans[i]+key%(94);

	}
	return ans;
}
#include <iostream>
using namespace std;

// 33 to 126 are printable characters

string encrypt(string s,int key)
{
	string ans=s;
	key=key%94;
	int val;
	for(int i=0;i<s.length();i++)
	{
		val=ans[i]+key;
		if(val>126)
			val-=94;
		ans[i]=val;
	}
	return ans;
}

string decrypt(string s,int key)
{
	string ans=s;
	key=key%94;
	int val;
	for(int i=0;i<s.length();i++)
	{
		val=ans[i]-key;
		if(val<33)
			val=93+val;
		ans[i]=val;
	}
	return ans;
}

void check_encrypt()
{
	string s="sahil";
	char a=33,b=126;
	
	for(int key=0;key<=110;key++)
		cout<<key<<" "<<encrypt(s,key)<<endl;
}

void check_program()
{
	string s="sahil";
	for(int i=0;i<=110;i++)
	{
		cout<<i<<" "<<decrypt(encrypt(s,i),i)<<endl;
	}
}



int main()
{
	//cout<<decrypt("%qxy|",110)<<endl;
	check_program();
	return 0;
}
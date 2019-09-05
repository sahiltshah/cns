#include <iostream>
using namespace std;

// 33 to 126 are printable characters
int start=33,end=126;
string basetext,ciphertext;

string encrypt(string s,int key)
{
	ciphertext=basetext;
	key=key%94;

	if(key==0) return s;

	ciphertext=ciphertext.substr(key,ciphertext.size())+ciphertext.substr(0,key);

	for(int i=0;i<s.size();i++)
		s[i]=ciphertext[s[i]-start];
	return s;
}


string decrypt(string s,int key)
{
	key=key%94;
	key=94-key;	
	return encrypt(s,key);
}

void check_program() // function to check the cipher in differet
{
	string s="sahil";
	cout<<"ASCII 33 to 126 are printable characters thus total char: "<<(126-33+1);
	cout<<"\nbase string is: "<<s<<endl;
	cout<<"Key | BaseString -> Encrypted -> Decrypted"<<endl;
	
	
	for(int i=0;i<=110;i++)
	{
		cout<<i<<" | "<<s<<" -> "<<encrypt(s,i)<<" -> "<<decrypt(encrypt(s,i),i)<<endl;
	}
	
}



int main()
{
	int count =0;
	basetext.resize((end-start+1),'#');
	ciphertext.resize((end-start+1),'#');
	for(int i=33;i<=126;i++)
		basetext[count++]=i;
	
	check_program();
	return 0;
}
#include <iostream>
using namespace std;
int main()
{
	char c;
	for(int i=0;i<256;i++)
	{
		c=i;
		cout<<i<<" "<<c<<" ";
		if(i%5==0)
			cout<<endl;
	}
}

// 33 to 126 are printable characters
														DIFFIE HELLMAN

#include <bits/stdc++.h>
#include <gmp.h>
using namespace std;


void generator(mpz_t g,mpz_t n)
{
	mpz_t arr[100],p,t,n0;
	mpz_inits(t,p,n0,0);
	mpz_sub_ui(n0,n,1);
	for(int i=0;i<100;i++)
	{
		mpz_inits(arr[i],NULL);
		//mpz_set_ui(arr[i],)
	}
	unsigned long int num = mpz_get_ui(n);
	vector<bool> v(num+1,true);
	v[0] = v[1] = false;
	int ind = 0,i = 2,m;
	m = num-1;
	for(;i*i<=num&&m!=1;i++)
	{
		if(v[i])
		{
			for(int j=2*i;j<=num;j+=i)
			{
				v[j] = false;
			}
			if(m%i==0)
			{
				mpz_set_ui(arr[ind],i);ind++;
			}
		}
		while(m%i==0)
		{
			m/=i;
		}
	}
	for(;i<=num&&m!=1;i++)
	{
		if(v[i]&&m%i==0)
		{
			mpz_set_ui(arr[ind],i);ind++;
			while(m%i==0)
				m/=i;
		}
	}
	m = num-1;bool  f;
	for(i=2;i<num;i++)
	{
		f = true;
		for(int j=0;j<ind;j++)
		{
			mpz_div(p,n0,arr[j]);
			mpz_set_ui(t,i);
			mpz_powm(t,t,p,n);
			if(mpz_cmp_ui(t,1)==0)
			{
				f = false;break;
			}
		}
		if(f)
		{
			mpz_set_ui(g,i);
			return;
		}
	}
}

int main(int argc, char const *argv[])
{
	mpz_t n,g,xA,xB,yA,yB,K1,K2;
	mpz_inits(n,g,xA,xB,yA,yB,K1,K2,NULL);
	//cout<<"Enter the order of field"<<endl;
	//gmp_scanf("%Zd",n);
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,time(NULL));
	mpz_urandomb(n,state,15);
	mpz_nextprime(n,n);
	gmp_printf("n = %Zd\n",n);
	generator(g,n);
	if(mpz_cmp_ui(g,0)==0)
	{
		printf("No generator for this field\n");exit(0);
	}
	else
	{
		gmp_printf("generator = %Zd\n",g);
	}

	//cout<<"Enter x1 and x2"<<endl;
	//gmp_scanf("%Zd%Zd",xA,xB);
	mpz_urandomb(xA,state,10);
	mpz_urandomb(xB,state,10);
	gmp_printf("xA = %Zd\nxB = %Zd\n",xA,xB);

	mpz_powm(yA,g,xA,n);
	mpz_powm(yB,g,xB,n);

	gmp_printf("yA = %Zd\nyB = %Zd\n",yA,yB);

	mpz_powm(K1,yA,xB,n);
	mpz_powm(K2,yB,xA,n);
	gmp_printf("K1 = %Zd\nK2 = %Zd\n",K1,K2);

	cout<<"Equal? = "<<mpz_cmp(K1,K2)<<endl;
	return 0;
}


------------------------------------------------------------------------------------------------------------------------------------------------------------

DIFFIE HELLMAN USING ECC

#include <bits/stdc++.h>
#include "ECC.h"
using namespace std;

// y3 = x2-3x+b


int main(int argc, char const *argv[])
{
	mpz_t xA,xB,n;
	mpz_inits(xA,xB,n,NULL);

	//mpz_set_str(n,"6277101735386680763835789423176059013767194773182842284081",10);

	mpz_set_str(n,"1000",10);

	Point G("188da80e b03090f6 7cbf20eb 43a18800 f4ff0afd 82ff1012",
		"07192b95 ffc8da78 631011ed 6b24cdd5 73f977a1 1e794811",
		"-3",
		"64210519 e59c80e7 0fa7e9ab 72243049 feb8deec c146b9b1",
		"6277101735386680763835789423207666416083908700390324961279",
		10,16,16,16,10),PA,PB,K1,K2;

	cout<<G<<endl;

	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,time(NULL));
	mpz_urandomm(xA,state,n);
	mpz_urandomm(xB,state,n);

	PA = G*xA;
	PB = G*xB;
	gmp_printf("nA = %Zd\nnB = %Zd\n",xA,xB);
	cout<<"PA = "<<PA<<endl<<"PB = "<<PB<<endl;

	K1 = PA*xB;
	K2 = PB*xA;

	cout<<"K1 = "<<K1<<endl<<"K2 = "<<K2<<endl;

	if(K1==K2)
	{
		cout<<"YES\n";
	}
	else
	{
		cout<<"NO\n";
	}

	return 0;
}

-------------------------------------------------------------------------------------------------------------------------------------------------


ECC.h

#include <bits/stdc++.h>
#include <gmp.h>
using namespace std;

#ifndef __ECC
#define __ECC

// this ECC curve is over Zp

#define inf "6277101735386680763835789423207666416083908700390324961279000000"


void mul_inv(mpz_t inv,mpz_t a1,mpz_t p1)
{
	mpz_t x0,x1,y0,y1,q,g,r,t,a,p;
	mpz_inits(x0,x1,y0,y1,q,g,r,t,a,p,NULL);
	mpz_set(a,a1);
	mpz_set(p,p1);
	mpz_set_ui(x0,1);
	mpz_set_ui(x1,0);
	mpz_set_ui(y0,0);
	mpz_set_ui(y1,1);
	while(mpz_cmp_ui(p,1)!=0)
	{
		mpz_mod(r,a,p);
		mpz_div(q,a,p);
		mpz_set(a,p);
		mpz_set(p,r);
		mpz_set(t,x1);
		mpz_mul(x1,x1,q);
		mpz_sub(x1,x0,x1);
		mpz_set(x0,t);
		mpz_set(t,y1);
		mpz_mul(y1,y1,q);
		mpz_sub(y1,y0,y1);
		mpz_set(y0,t);
	}

	mpz_set(inv,x1);
}


class Point
	{
	private:
		mpz_t a,b,x,y,p,INF;
	public:
		Point()
		{
			mpz_inits(a,b,x,y,p,INF,NULL);
			mpz_set_str(INF,inf,10);
		}
		Point(const char* a1,const char* b1,const char* a,const char* b,const char* p,int bA=10,int bB=10,int bX=10,int bY=10,int bP=10)//,bool negA=false,bool negB=false)
		{

			mpz_inits(this->a,this->b,this->x,this->y,this->p,this->INF,NULL);
			mpz_set_str(x,a1,bX);
			mpz_set_str(y,b1,bY);
			mpz_set_str(this->a,a,bA);
			mpz_set_str(this->b,b,bB);
			mpz_set_str(this->p,p,bP);
			mpz_set_str(INF,inf,10);
		}
		Point(mpz_t a1,mpz_t b1,mpz_t a,mpz_t b,mpz_t p)
		{

			mpz_inits(this->a,this->b,this->x,this->y,this->p,this->INF,NULL);
			mpz_set(x,a1);
			mpz_set(y,b1);
			mpz_set(this->a,a);
			mpz_set(this->b,b);
			mpz_set(this->p,p);
			mpz_set_str(INF,inf,10);
		}
		bool isInf()
		{
			return (mpz_cmp(this->x,INF)==0)&&(mpz_cmp(this->y,INF)==0);
		}
		bool operator ==(Point p1)
		{
			return (mpz_cmp(this->x,p1.x)==0)&&(mpz_cmp(this->y,p1.y)==0);
		}
		void operator=(Point p)
		{
			mpz_set(this->x,p.x);
			mpz_set(this->y,p.y);
			mpz_set(this->p,p.p);
			mpz_set(this->a,p.a);
			mpz_set(this->b,p.b);
		}
		Point operator +(Point p1)
		{
			mpz_t slope,num,den,t;
			mpz_inits(slope,num,den,t,NULL);
			if(*this==p1)
			{
				mpz_mul(t,p1.x,p1.x);
				mpz_mul_ui(t,t,3);
				mpz_add(t,t,a);
				mpz_mod(t,t,p);
				mpz_set(num,t);
				mpz_set(t,p1.y);
				mpz_mul_ui(t,t,2);
				mpz_mod(t,t,p);
				mpz_set(den,t);
			}
			else
			{
				mpz_set(t,p1.x);
				mpz_sub(t,t,this->x);
				mpz_mod(den,t,p);
				mpz_set(t,p1.y);
				mpz_sub(t,t,this->y);
				mpz_mod(num,t,p);
			}
			if(mpz_cmp_ui(den,0)==0)
			{
				return Point(INF,INF,a,b,p); // The No point
			}
			else
			{
				mpz_t xr,yr;
				mpz_inits(xr,yr,NULL);
				mul_inv(t,den,p);
				mpz_mul(slope,num,t);
				mpz_mod(slope,slope,p);
				mpz_mul(t,slope,slope);
				mpz_sub(t,t,this->x);
				mpz_sub(t,t,p1.x);
				mpz_add(t,t,p);
				mpz_mod(xr,t,p);
				mpz_sub(t,this->x,xr);
				mpz_add(t,t,p);
				mpz_mul(t,t,slope);
				mpz_mod(t,t,p);
				mpz_sub(t,t,this->y);
				mpz_add(t,t,p);
				mpz_mod(yr,t,p);
				return Point(xr,yr,a,b,p);
			}
		}
		Point operator*(mpz_t m1)
		{
			mpz_t m;
			mpz_inits(m,NULL);
			mpz_set(m,m1);
			Point p;
			p = *this;
			while(mpz_cmp_ui(m,1)!=0)
			{
				p = p+(*this);
				mpz_sub_ui(m,m,1);
			}
			return p;
		}
		friend ostream& operator <<(ostream& out,Point p)
		{
			if(p.isInf())
			{
				out<<"( INF , INF )";
			}
			else
			gmp_printf("( %Zd , %Zd )",p.x,p.y);
			return out;
		}
	};


#endif


-------------------------------------------------------------------------------------------------------------------------------------------------

egcd.c 


#include <stdio.h>
#include <gmp.h>

int main(){

	mpz_t number1,number2,coef1,coef2,tempquotient,tempx1,tempy2,tempx2,tempy1,remainder,temp1,temp2;
	mpz_inits(number1,number2,coef1,coef2,tempquotient,tempx1,tempy1,tempx2,tempy2,remainder,temp1,temp2,NULL);
	mpz_set_ui(tempx1,1);
	mpz_set_ui(tempx2,0);
	mpz_set_ui(tempy1,0);
	mpz_set_ui(tempy2,1);
	mpz_set_ui(remainder,1);
				
	
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	unsigned long seed;
	seed  = time(NULL);
	gmp_randseed_ui(state,seed);
	mpz_set_ui(max,1000);
	mpz_rrandomm(number1,state,max);
	mpz_rrandomm(number2,state,max);                           // int bits =10; mpz_rrandomb(a,state,bits);			
	
	gmp_printf(" The numbers are %Zd",number1);
	gmp_printf(" and %Zd\n",number2);
	
	if(mpz_cmp(number1,number2) < 0)mpz_swap(number1,number2);
	
	while(mpz_cmp_ui(remainder,0) > 0){
	
		/* normal euclid gcd algoritm */
		mpz_div(tempquotient,number1,number2);
		mpz_mod(remainder,number1,number2);
		mpz_set(number1,number2);
		mpz_set(number2,remainder);
		
		/* extended */
		mpz_set(temp1,tempx1);
		mpz_mul(temp2,tempquotient,tempx2);
		mpz_sub(temp1,temp1,temp2);
		mpz_set(tempx1,tempx2);
		mpz_set(tempx2,temp1);
		
		mpz_set(temp1,tempy1);
		mpz_mul(temp2,tempquotient,tempy2);
		mpz_sub(temp1,temp1,temp2);
		mpz_set(tempy1,tempy2);
		mpz_set(tempy2,temp1);
		
	}
	
	

	gmp_printf("\n%Zd %Zd \n" ,tempx1,tempy1);
	return 0;
}


/*

	testcases: 240,46 -> -9 ,47
			   35,15  -> 1,-2
			   1,1 -> 0,1
			   */


------------------------------------------------------------------------------------------------------------------------------------------------

El_Gamal.cpp

#include <bits/stdc++.h>
#include <gmp.h>
using namespace std;


void generator(mpz_t g,mpz_t n)
{
	mpz_t arr[100],p,t,n0;
	mpz_inits(t,p,n0,0);
	mpz_sub_ui(n0,n,1);
	for(int i=0;i<100;i++)
	{
		mpz_inits(arr[i],NULL);
		//mpz_set_ui(arr[i],)
	}
	unsigned long int num = mpz_get_ui(n);
	vector<bool> v(num+1,true);
	v[0] = v[1] = false;
	int ind = 0,i = 2,m;
	m = num-1;
	for(;i*i<=num&&m!=1;i++)
	{
		if(v[i])
		{
			for(int j=2*i;j<=num;j+=i)
			{
				v[j] = false;
			}
			if(m%i==0)
			{
				mpz_set_ui(arr[ind],i);ind++;
			}
		}
		while(m%i==0)
		{
			m/=i;
		}
	}
	for(;i<=num&&m!=1;i++)
	{
		if(v[i]&&m%num==0)
		{
			mpz_set_ui(arr[ind],i);ind++;
			while(m%i==0)
				m/=i;
		}
	}
	m = num-1;bool  f;
	for(i=2;i<num;i++)
	{
		f = true;
		for(int j=0;j<ind;j++)
		{
			mpz_div(p,n0,arr[j]);
			mpz_set_ui(t,i);
			mpz_powm(t,t,p,n);
			if(mpz_cmp_ui(t,1)==0)
			{
				f = false;break;
			}
		}
		if(f)
		{
			mpz_set_ui(g,i);
			return;
		}
	}
}

int main(int argc, char const *argv[])
{
	mpz_t n,g,xA,xB,yA,yB,K,c1,c2,k,t,t1,num;
	mpz_inits(n,g,xA,xB,yA,yB,K,c1,c2,k,t,t1,num,NULL);
	//cout<<"Enter the order of field"<<endl;
	//gmp_scanf("%Zd",n);
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,time(NULL));
	mpz_urandomb(n,state,20);
	mpz_nextprime(n,n);
	gmp_printf("n = %Zd\n",n);
	generator(g,n);
	if(mpz_cmp_ui(g,0)==0)
	{
		printf("No generator for this field\n");exit(0);
	}
	else
	{
		gmp_printf("generator = %Zd\n",g);
	}
	mpz_urandomb(xA,state,20);
	mpz_urandomb(xB,state,20);
	gmp_printf("xA = %Zd\nxB = %Zd\n",xA,xB);

	mpz_powm(yA,g,xA,n);
	mpz_powm(yB,g,xB,n);

	gmp_printf("yA = %Zd\nyB = %Zd\n",yA,yB);

	
	mpz_sub_ui(t,n,1);
	mpz_urandomm(k,state,t);
	mpz_powm(K,yB,k,n);
	cout<<"Enter a message\n";
	gmp_scanf("%Zd",num);
	mpz_powm(c1,g,k,n);
	mpz_mul(c2,K,num);
	mpz_mod(c2,c2,n);

	gmp_printf("Encrypted = %Zd\n",c2);

	mpz_powm(c1,c1,xB,n);
	mpz_gcdext(t,t,t1,n,c1);
	mpz_mul(t,t1,c2);
	mpz_mod(t,t,n);
	gmp_printf("Decrypted = %Zd\n",t);
	return 0;
}


-------------------------------------------------------------------------------------------------------------------------------------------------

ELGAmal DSS .cpp

#include <bits/stdc++.h>
#include <gmp.h>
using namespace std;

void generator(mpz_t g,mpz_t n)
{
	mpz_t arr[100],p,t,n0;
	mpz_inits(t,p,n0,0);
	mpz_sub_ui(n0,n,1);
	for(int i=0;i<100;i++)
	{
		mpz_inits(arr[i],NULL);
	}
	unsigned long int num = mpz_get_ui(n);
	vector<bool> v(num+1,true);
	v[0] = v[1] = false;
	int ind = 0,i = 2,m;
	m = num-1;
	for(;i*i<=num&&m!=1;i++)
	{
		if(v[i])
		{
			for(int j=2*i;j<=num;j+=i)
			{
				v[j] = false;
			}
			if(m%i==0)
			{
				mpz_set_ui(arr[ind],i);ind++;
			}
		}
		while(m%i==0)
		{
			m/=i;
		}
	}
	for(;i<=num&&m!=1;i++)
	{
		if(v[i]&&m%num==0)
		{
			mpz_set_ui(arr[ind],i);ind++;
			while(m%i==0)
				m/=i;
		}
	}
	m = num-1;bool  f;
	for(i=2;i<num;i++)
	{
		f = true;
		for(int j=0;j<ind;j++)
		{
			mpz_div(p,n0,arr[j]);
			mpz_set_ui(t,i);
			mpz_powm(t,t,p,n);
			if(mpz_cmp_ui(t,1)==0)
			{
				f = false;break;
			}
		}
		if(f)
		{
			mpz_set_ui(g,i);
			return;
		}
	}
}

int main(int argc, char const *argv[])
{
	mpz_t n,g,xA,xB,yA,yB,t,t1,num;
	mpz_inits(n,g,xA,xB,yA,yB,t,t1,num,NULL);
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state,time(NULL));
	mpz_urandomb(n,state,20);
	mpz_nextprime(n,n);
	gmp_printf("n = %Zd\n",n);
	generator(g,n);
	if(mpz_cmp_ui(g,0)==0)
	{
		printf("No generator for this field\n");exit(0);
	}
	else
	{
		gmp_printf("generator = %Zd\n",g);
	}
	mpz_urandomb(xA,state,20);
	mpz_urandomb(xB,state,20);
	gmp_printf("xA = %Zd\nxB = %Zd\n",xA,xB);

	mpz_powm(yA,g,xA,n);
	mpz_powm(yB,g,xB,n);

	gmp_printf("yA = %Zd\nyB = %Zd\n",yA,yB);

	mpz_t S1,S2,n0,k,t2;
	mpz_inits(S1,S2,n0,k,t2,NULL);



	mpz_sub_ui(n0,n,1);
	do
	{
		mpz_urandomm(k,state,n0);
		mpz_gcd(t,k,n0);
	}while(mpz_cmp_ui(t,1)!=0);

	printf("Enter a number\n");
	gmp_scanf("%Zd",num);

	mpz_powm(S1,g,k,n);

	mpz_gcdext(t,t1,t2,n0,k);

	mpz_mul(t,xA,S1);
	mpz_mod(t,t,n0);

	mpz_sub(t,num,t);
	mpz_mul(t,t,t2);
	mpz_mod(S2,t,n0);

	gmp_printf("S1 = %Zd\nS2 = %Zd\n",S1,S2);

	mpz_powm(t,g,num,n);

	mpz_powm(t1,yA,S1,n);
	mpz_powm(t2,S1,S2,n);
	mpz_mul(t1,t1,t2);
	mpz_mod(t1,t1,n);

	gmp_printf("Ver1 = %Zd\nVer2 = %Zd\n",t,t1);

	return 0;
}

-------------------------------------------------------------------------------------------------------------------------------------------------



extended euclidean.c


/*
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	unsigned long seed;
	seed = time(NULL);

		mpz_set_ui(max,100000);
		mpz_urandomm(a,state,max);

	//the following method is preferable*/


#include <stdio.h>
#include <gmp.h>
#include <time.h>

void exeuclid(mpz_t a,mpz_t b,mpz_t x,mpz_t y,mpz_t d)
{
	//gmp_printf("euclid called\n");
	mpz_t x1,x2,y1,y2,q,r;
	mpz_inits(x1,x2,y1,y2,q,r,NULL);
		mpz_set_ui(x2,1);
		mpz_set_ui(y1,1);
		while(mpz_cmp_ui(b,0) > 0)
		{
			mpz_fdiv_q(q,a,b); 
			mpz_set(r,a); //r = a
			mpz_submul(r,q,b); // r = a- qb
			mpz_set(x,x2); //x = x2
			mpz_submul(x,q,x1); // x = x2 - qx1
			mpz_set(y,y2);
			mpz_submul(y,q,y1);
			mpz_set(a,b);
			mpz_set(b,r);
			mpz_set(x2,x1);
			mpz_set(x1,x);
			mpz_set(y2,y1);
			mpz_set(y1,y);
		}
		mpz_set(d,a);
		mpz_set(x,x2);
		mpz_set(y,y2);
	
}
void main()
{
	mpz_t a,b,d,x,y;
	mpz_inits(a,b,d,x,y,NULL);
	
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	unsigned long seed;
	seed = time(NULL);
	gmp_randseed_ui(state,seed);
	/*
		mpz_set_ui(max,100000);
		mpz_urandomm(a,state,max);*/

	//the following method is preferable
	int bits = 16;
	mpz_rrandomb(a,state,bits);                                                                    
	mpz_rrandomb(b,state,bits);
	if(mpz_cmp_ui(b,0) == 0)
	{//gmp_printf("777777777777777\n");
			mpz_set(d,a);
			mpz_set_ui(x,1);
			mpz_set_ui(y,0);
			gmp_printf("%Zd = %Zd*%Zd + %Zd*%Zd",d,a,x,b,y);
	}
	else
	{//gmp_printf("Else");
	mpz_t a1,b1;
		mpz_init_set(a1,a);
		mpz_init_set(b1,b);
		exeuclid(a1,b1,x,y,d);
		gmp_printf("%Zd = (%Zd)*(%Zd) + (%Zd)*(%Zd)\n",d,a,x,b,y);
	}

}



--------------------------------------------------------------------------------------------------------------------------------------------------

gcd.c


#include <stdio.h>
#include <gmp.h>
 #include <time.h>

int main ()
{    
  
	mpz_t a, b, x, y, q, r, r1, r2, s, s1, s2, t, t1, t2;
	mpz_inits (a, b, x, y, q, r, r1, r2, s, s1, s2, t, t1, t2, NULL);

	
	gmp_scanf ("%Zd", a);
	gmp_scanf ("%Zd", b);

	if(mpz_cmp(a, b) < 0)
	{
		mpz_swap(a ,b);
	}	

	mpz_set(r1, a);
	mpz_set(r2, b);
	mpz_set_ui(s1, 1);
	mpz_set_ui(t1, 0);
	mpz_set_ui(s2, 0);
	mpz_set_ui(t2, 1);
	

	while(mpz_cmp_ui(r2, 0) > 0)
	{
		mpz_fdiv_q(q, r1, r2);

		mpz_mod(r, r1, r2);

		mpz_set(r1, r2);
		mpz_set(r2, r);

		mpz_mul(s, q, s2);
		mpz_sub(s, s1, s);

		mpz_set(s1, s2);
		mpz_set(s2, s);

		mpz_mul(t, q, t2);
		mpz_sub(t, t1, t);

		mpz_set(t1, t2);
		mpz_set(t2, t);
	}

	gmp_printf("GCD is %Zd \n s is %Zd \n t  is %Zd \n ", r1, s1, t1);
}






---------------------------------------------------------------------------------------------------------------------------------------------


modular_arithmetic.c


#include <stdio.h>
#include <gmp.h>
#include <time.h>

int main()
{

	mpz_t a,b,c,e,d,max,m;
	mpz_inits(a,b,c,d,e,max,m,NULL);
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	unsigned long seed;
	seed  = time(NULL);
	gmp_randseed_ui(state,seed);
	mpz_set_ui(max,1000);
	mpz_urandomm(a,state,max);
	mpz_urandomm(b,state,max);                           // int bits =10; mpz_rrandomb(a,state,bits);		
	mpz_urandomm(m,state,max);	
	
	gmp_printf("The selected numbers are %Zd, %Zd in mod %Zd\n",a,b,m);
	
	mpz_mod(a,a,m);
	mpz_mod(b,b,m);
	mpz_add(c,a,b);
	mpz_sub(d,a,b);
	mpz_mul(e,a,b );
	mpz_mod(e,e,m);
	mpz_mod(c,c,m);
	mpz_mod(d,d,m);
	
	
	
	gmp_printf("\nsum modulo is %Zd \nsub modulo is %Zd \nmul modulo %Zd\n",c,d,e);
	
	
	
		
	return 0;
}


-------------------------------------------------------------------------------------------------------------------------------------------------

poly div 


#include <bits/stdc++.h>
using namespace std;
void printBinary(int n)
{
	int k;
	for (int c = 7; c >= 0; c--)
	{
		k = n >> c;

		if (k & 1)
		printf("1");
		else
		printf("0");
	}

	printf("\n");
}

int getMSB(int n)
{
	return log2(n);
}



int main()
{
	//GF(2^8);
	int a = 0,b = 0;
	int n = 8;
	char ch;

	
	cout<<"Enter the Polynomial a(x)"<<endl;
	for(int i = n-1; i >= 0; i --)
	{
		cin>>ch;
		if(ch == '1')
		{
			a = a | (1 << i);
		}
	}

	
	cout<<"Enter the Polynomial b(x)"<<endl;
	
	for(int i = n-1; i >= 0; i --)
	{
		cin>>ch;
		if(ch == '1')
		{
			b = b | (1 << i);
		}
	}
	
	int highest_deg_a = getMSB(a);
	int highest_deg_b = getMSB(b);
	int quo = 0;
	cout<<highest_deg_a<<"::"<<highest_deg_b<<endl;

	while(highest_deg_a >= highest_deg_b)	//continue doing division
	{
		quo = quo | (1 << (highest_deg_a - highest_deg_b));
	
		//Multiplication of x^(highest_deg_a - highest_deg_b) and b
		int temp = b << (highest_deg_a - highest_deg_b);
		a = a ^ temp;


		highest_deg_a = getMSB(a);
		highest_deg_b = getMSB(b);
	}

	cout<<"Quotient is "<<endl;
	printBinary(quo);

	cout<<"Remainder is "<<endl;
	printBinary(a);

	
}


-----------------------------------------------------------------------------------------------------------------------------------------------
poly egcd




#include <bits/stdc++.h>
using namespace std;
void printBinary(int n)
{
	int k;
	for (int c = 7; c >= 0; c--)
	{
		k = n >> c;

		if (k & 1)
		printf("1");
		else
		printf("0");
	}

	printf("\n");
}

int getMSB(int n)
{
	return log2(n);
}

int polydiv(int a, int b, int &quo, int &rem)
{

	int highest_deg_a = getMSB(a);
	int highest_deg_b = getMSB(b);
	quo = 0;

	while(highest_deg_a >= highest_deg_b)	//continue doing division
	{
		quo = quo | (1 << (highest_deg_a - highest_deg_b));
	
		//Multiplication of x^(highest_deg_a - highest_deg_b) and a
		int temp = b << (highest_deg_a - highest_deg_b);
		a = a ^ temp;


		highest_deg_a = getMSB(a);
		highest_deg_b = getMSB(b);
	}
	rem = a;
}

void polymul(int a, int b, int &res)
{
	int c = a;
	res = 0;

	if(b & 1) 
	res = a;
	
	int highest_deg = getMSB(b);
	int n = 8;

	for(int i = 1; i <= highest_deg; i++)
	{
		if(c & (1 << (n - 1)))
		{
			c = c << 1;
			c = c ^ 27;			// 27(10) = 00011011(2);
		}
		else
		{
			c = c << 1;
		}
		
		if(((1 << i) & b))
		{
			res = res ^ c;			
		}		
	}	
}

int main()
{
	//GF(2^8);
	int a = 0,b = 0;
	int n = 8;
	char ch;

	
	cout<<"Enter the modular Polynomial a(x)"<<endl;
	for(int i = 8; i >= 0; i --)
	{
		cin>>ch;
		if(ch == '1')
		{
			a = a | (1 << i);
		}
	}

	
	cout<<"Enter the Polynomial b(x)"<<endl;
	
	for(int i = n-1; i >= 0; i --)
	{
		cin>>ch;
		if(ch == '1')
		{
			b = b | (1 << i);
		}
	}
	
	int r1 = a, r2 = b;
	int q, r, s2 = 0, t2 = 1, s1 = 1, t1 = 0, s, t;

	while(r2 > 0)
	{
		polydiv(r1, r2, q, r);
		
		r1 = r2;
		r2 = r;

		int temp;
		//t = t1-qt2;t1= t2;t2= t;
		polymul(q, t2, temp);
		

		t = t1 ^ temp;
		t1 = t2;
		t2 = t;
		
	}

	cout<<"Modular Multiplicative inverse is  "<<endl;
	printBinary(t1);

	

	
}



------------------------------------------------------------------------------------------------------------------------------------------------

polygcd

#include <bits/stdc++.h>
using namespace std;
void printBinary(int n)
{
	int k;
	for (int c = 7; c >= 0; c--)
	{
		k = n >> c;

		if (k & 1)
		printf("1");
		else
		printf("0");
	}

	printf("\n");
}

int getMSB(int n)
{
	return log2(n);
}

int polydiv(int a, int b, int &quo, int &rem)
{

	int highest_deg_a = getMSB(a);
	int highest_deg_b = getMSB(b);
	quo = 0;
//	cout<<highest_deg_a<<"::"<<highest_deg_b<<endl;

	while(highest_deg_a >= highest_deg_b)	//continue doing division
	{
		quo = quo | (1 << (highest_deg_a - highest_deg_b));
	
		//Multiplication of x^(highest_deg_a - highest_deg_b) and a
		int temp = b << (highest_deg_a - highest_deg_b);
		a = a ^ temp;


		highest_deg_a = getMSB(a);
		highest_deg_b = getMSB(b);
	}

//	cout<<"Quotient is "<<endl;
//	printBinary(quo);

//	cout<<"Remainder is "<<endl;
//	printBinary(a);
	rem = a;
}

int main()
{
	//GF(2^8);
	int a = 0,b = 0;
	int n = 8;
	char ch;

	
	cout<<"Enter the Polynomial a(x)"<<endl;
	for(int i = n-1; i >= 0; i --)
	{
		cin>>ch;
		if(ch == '1')
		{
			a = a | (1 << i);
		}
	}

	
	cout<<"Enter the Polynomial b(x)"<<endl;
	
	for(int i = n-1; i >= 0; i --)
	{
		cin>>ch;
		if(ch == '1')
		{
			b = b | (1 << i);
		}
	}
	
	int r1 = a, int r2 = b;

	while(r2 > 0)
	{
		polydiv(r1, r2, q, r);
		
		cout<<"Quotient is ";
		printBinary(q);

		cout<<"Remainder is ";
		printBinary(r);

		r1 = r2;
		r2 = r;
	}

	cout<<"GCD is "endl;
	printBinary(r1);

	
}

------------------------------------------------------------------------------------------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;
void printBinary(int n)
{
	int k;
	for (int c = 7; c >= 0; c--)
	{
		k = n >> c;

		if (k & 1)
		printf("1");
		else
		printf("0");
	}

	printf("\n");
}
int main()
{
	//GF(2^8);
	int a = 0,b = 0;
	int n = 8;
	char ch;
	cout<<"Enter the Polynomial a(x)"<<endl;
	for(int i = n-1; i >= 0; i --)
	{
		cin>>ch;
		if(ch == '1')
		{
			a = a | (1 << i);
		}
	}

	cout<<"Enter the Polynomial b(x)"<<endl;
	int highest_deg;
	bool flag = true;
	for(int i = n-1; i >= 0; i --)
	{
		cin>>ch;
		if(ch == '1')
		{
			if(flag)
			highest_deg = i, flag = false;

			b = b | (1 << i);
		}
	}
	
	printBinary(a);
	printBinary(b);

	int c = a;
	int res = 0;

	if(b & 1) 
	res = a;

	for(int i = 1; i <= highest_deg; i++)
	{
		if(c & (1 << (n - 1)))
		{
			c = c << 1;
			c = c ^ 27;			// 27(10) = 00011011(2);
		}
		else
		{
			c = c << 1;
		}
		
		if(((1 << i) & b))
		{
			res = res ^ c;			
		}		
	}
	cout<<"Multiplication is "<<endl;
	printBinary(res);
}



------------------------------------------------------------------------------------------------------------------------------------------------



rsa.c  


#include <stdio.h>
#include <gmp.h>
 #include <time.h>

int main ()
{    
  
	mpz_t p,q,ph, p1, q1, r, e,d,b, g,n, m, pl,c;
	mpz_inits (p,q,ph,p1, q1,e,d,b,g,m,c, pl,NULL);
	mpz_inits(r, n, NULL);

  	gmp_randstate_t state;
	gmp_randinit_mt(state);
	unsigned long seed;
	seed = time(NULL);
	gmp_randseed_ui(state,seed);
	int bits = 1024;
	mpz_rrandomb(r,state,bits);                                                                    
	
	mpz_nextprime (p , r);
	mpz_nextprime (q , p);
	mpz_mul(n, p, q);

	mpz_set_ui (p1, 1 );
	mpz_set_ui (q1 , 1 );
	mpz_set_ui (e , 17 );
	mpz_set_ui (m , 89 );

	mpz_sub(p1,p,p1);
	mpz_sub(q1,q,q1);
	mpz_mul(ph,p1, q1);

  	mpz_gcdext ( g , d , b , e , ph );
	mpz_powm (c, m, e, n);
	mpz_powm (pl, c, d, n);



	gmp_printf("Two primes are ===  \n");
	gmp_printf("p == %Zd \nq ==  %Zd\n",p, q);
	gmp_printf("p-1 ==  %Zd \nq-1 == %Zd\n",p1, q1);
	gmp_printf("ph == %Zd \n ",ph);
	gmp_printf("e == %Zd \n ", e);
	gmp_printf("d == %Zd \n ", d);
	gmp_printf("Cipher Text  == %Zd \n ", c); 
	gmp_printf("Plain Text  == %Zd \n ", pl);



}



---------------------------------------------------------------------------------------------------------------------------------------------



ceasar

#include <bits/stdc++.h>
using namespace std;
void cipher(string &a,int k){
	for(int i = 0;i < a.length();i++)
	{	
		a[i] = ((a[i] + k - 'a')%26) + 'a';
	}
}

void decipher(string &a,int k){
	for(int i = 0;i < a.length();i++)
	{	
		a[i] = (a[i] - k - 'a' + 26)%26 + 'a';
	}
}

int main()
{
	int k;
	cout<<"Enter the key"<<endl;
	cin>>k;
	string a;
	cin>>a;
	cipher(a,k);
	cout<<"cipher text is "<<a<<endl;
	decipher(a,k);
	cout<<"deciphered text is "<<a<<endl;
	return 0;
}


------------------------------------------------------------------------------------------------------------------------------------------------
monoalphabetic

#include <bits/stdc++.h>
using namespace std;
void cipher(string &a,string key)
{
	for(int i = 0;i < a.length();i++){
		a[i] = key[a[i] - 'a'] ;
	}
}
void decipher(string &a,string key)
{
	for(int j = 0;j < a.length();j++)
	{
		for(int i = 0;i < key.length();i++){
			if(key[i] == a[j]){
				a[j] = i + 'a' -32;
				break;
			} 
		}
	}
}
int main()
{
	string a;
	string key = "dkvqfibjwpescxhtmyauolrgzn";
	cout<<"Enter plain text"<<endl;
	cin>>a;
	cipher(a,key);
	cout<<"cipher text is "<<a<<endl;
	decipher(a,key);
	cout<<"decipher text is "<<a<<endl;
	return 0;
}


------------------------------------------------------------------------------------------------------------------------------------------------




playfair


#include <bits/stdc++.h>
using namespace std;
vector<int> find(char lookup[5][5],char c)
{
	vector<int> res;
	for(int i=0;i<5;i++)
	{
		for(int j=0;j<5;j++)
		{
			if(lookup[i][j] == c)
			{
				res.push_back(i);
				res.push_back(j);
				return res;
			}
		}
	}
}
void cipher(char lookup[5][5],string &a)
{
	string temp = "";
	for(int i = 0;i < a.length();i = i + 1)
	{
		if(i + 1 < a.length())
		{
			if( a[i] == a[i+1])
			{
				temp = temp  + a[i] + 'x';
			}
			else if(a[i] != a[i+1])
			{
				temp = temp + a[i] + a[i+1];
				i++;
			}
		}
	}
	if(temp.length() %2 == 1) temp += 'x';
	cout<<temp<<"-------------------\n";int i,j,m,n;vector<int> ve;
	for(int k = 0; k < temp.length();k = k+2)
	{
		ve = find(lookup,a[k]);
		i = ve[0];
		j = ve[1];
		ve = find(lookup,a[k+1]);
		m = ve[0];
		n = ve[1];
		if(i == m)
		{ j = (j+1)%5;
			n = (n+1)%5;
		}
		if( j == n)
		{
			i = (i+1)%5;
			m = (m+1)%5;
		}
		cout<<lookup[m][j]<<lookup[i][n];
	}
	cout<<endl;
	
}

void decipher(string &a){
	cout<<a<<endl;
}

void fillup(char a[5][5],string key){
	int x = 0,y = 0;
	set <char> m;
	m.clear();
	for(int i = 0;i<key.length();i++){
		if(m.find(key[i]) != m.end())continue;
		if(key[i] == 'j' && m.find('i') != m.end())continue;
		else if(key[i] == 'j')
		{
			 key[i] = 'i';
		}
		m.insert(key[i]);
		a[x][y] = key[i];
		y++;
		if(y == 5){
			y = 0;
			x++;
		}
	}
	
	char b = 'a';
	bool flag = false;
	while(1){
		if(flag) break;
		for(int i = 0;i < 26;i++)
		{
			
			if(key.find(b + i) == string::npos)
			{
				a[x][y] = b+i;
				y++;
				if(y == 5)
				{
					y = 0;
					x++;
					if(x == 5)
					{
						flag = true;
						 break;
						 
						 }
				}
		
			}
			if(b + i == 'i') i++;
		}		
	}
}
int main()
{
	char a[5][5];
	string key,b;
	cin>>key;
	fillup(a,key);
	cout<<"Enter plain text"<<endl;
	cin>>b;
	string store;
	store = b;
	cipher(a,b);
	decipher(store);
	return 0;
}


----------------------------------------------------------------------------------------------------------------------------------------------

poly alphabetic

#include<bits/stdc++.h>
#include<string.h>
using namespace std;
int main()
{
string s="wearediscoveredsaveyourself";
//cin>>s;
string k="deceptive";
for(int i=0;i<s.length();i++)
{
s[i]=(s[i]-97+k[i%9]-97)%26 +97;
}
cout<<"encryption :"<<s<<endl;

for(int i=0;i<s.length();i++)
{
if(s[i]>k[i%9])
s[i]=(s[i]-k[i%9])+97;
else
s[i]=(s[i]+26-k[i%9])%26+97;
}
cout<<"decryption "<<s<<endl;


}


------------------------------------------------------------------------------------------------------------------------------------------------

rail fence


#include<bits/stdc++.h>
#include<string.h>
using namespace std;
int main()
{
string s;
string s1="";
string s2="";
cin>>s;
for(int i=0;i<s.length();i++)
{
if(i%2==0)
s1=s1+s[i];
else
s2=s2+s[i];
}
s=s1+s2;
cout<<"transposition cipher text encryption :"<<s<<endl;
s1="";
s2="";
int n=s.length();
int m=n;
int i=0;
s1=s.substr(0,(n+1)/2);
s2=s.substr((n+1)/2,(n-1)/2);
s="";
int j=0,k=0;
for(i=0;i<m;i++)
{
if(i%2==0)
s=s+s1[j++];
else
s=s+s2[k++];
}
cout<<"transposition cipher text decryption :"<<s<<endl;


}


------------------------------------------------------------------------------------------------------------------------------------------------

client c

#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

#define PORT 8080 

void main() 
{ 
	int opt = 1;  	
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	struct sockaddr_in address;
	int addrlen = sizeof(address);
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(PORT); 
	
	bind(sfd, (struct sockaddr*)&address, sizeof(address));
	listen(sfd, 10);

	int nsfd;
	while((nsfd = accept(sfd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) >= 0)
	{
	    char buffer[1024] = {0}; 
		recv(nsfd, buffer, sizeof(buffer), 0); 
		printf("%s\n", buffer); 

		char response[] = "Yeah got you";
		send(nsfd, response, strlen(response), 0); 

		close(nsfd);
	}
}


---------------------------------------------------------------------------------------------------------------------------------------------------


server c

#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <unistd.h>
#include <arpa/inet.h> 
#include <time.h>
#define PORT 8080 

int main() 
{ 
	struct sockaddr_in serv_addr; 
	memset(&serv_addr, '0', sizeof(serv_addr)); 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
	{ 
		printf("\nInvalid address / Address not supported \n"); 
		return -1; 
	} 

	while(1)
	{
		int sfd = socket(AF_INET, SOCK_STREAM, 0);
		if(connect(sfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) 
		{ 
			printf("\nConnection Failed \n"); 
			return -1; 
		}

		char msg[] = "Hey there";
		send(sfd, msg, strlen(msg), 0); 
		
		char buffer[1024] = {0};
		recv(sfd, buffer, sizeof(buffer), 0); 
		printf("%s\n",buffer ); 

		close(sfd);
		sleep(2);
	}

	return 0; 
}



-----------------------------------------------------------------------------------------------------------------------------------------------

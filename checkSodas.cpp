/*
Little program that starts up, 
queries the machine, and
prints the status of the sodas, X=Has, 0=None.
*/
#include <iostream>
#include <cmath>
#include "serialController.h"
using std::cout;
using std::endl;
using std::string;
using std::cin;
using namespace std;

int main()
{
string choice;
serialController p;

p.Open("/dev/ttyS0");

unsigned int buf = p.GetCanStatus();

//cout<<"RAW:"<<ios::hex<<buf<<endl;

for( unsigned int i=0; i<8; ++i )
	{
	if( buf & 1 )
		{
		cout<<'X';
		}
	else
		{
		cout<<'0';
		}
	buf = buf >> 1;
	}
cout<<endl;
return 0;
}

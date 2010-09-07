/*
Menu for testing the Soda Machine.
*/
#include <iostream>
#include "serialController.h"
using std::cout;
using std::endl;
using std::string;
using std::cin;
int main()
{
string choice;
bool run=true;
serialController p;
p.Open("/dev/ttyS0");
while( run )
	{
	cout	<<"************************\n"
			<<"*Please enter an Option*\n"
			<<"*1 Read the Buttons    *\n"
			<<"*2 Check the sodas     *\n"
			<<"*3 Vend a Soda         *\n"
			<<"*4 Exit                *\n"
			<<"************************\n"
			<<'?';
	cin >> choice;
	if( !cin )
		{
		run = false;
		}
	else if( choice == "1" )
		{
		int blocktime;
		cout<<"How long would you like to wait for a choice?(seconds)";
		cin >> blocktime;
		if( cin )
			{
			cout<<"Returned:"<<p.GetUserChoice( blocktime )<<endl;
			}
		else
			{
			run = false;
			}
		}
	else if( choice == "2" )
		{
		int buf = p.GetCanStatus();
		cout<<"**********\n*";
		for( int i=0; i<8; ++i )
			{
			cout << ( ( buf & 1 )?'X':'0' );
			buf >>= 1;
			}
		cout<<"*\n**********"<<endl<<endl;
		}
	else if ( choice == "3" )
		{
		int slot;
		cout<<"Which can?";
		cin>>slot;
		if( cin )
			{
			cout<<"Call returns:"<<p.VendCan( slot )<<endl;
			}
		else
			{
			run = false;
			}
		}
	else
		{
		run = false;
		}
	}
cout<<"Exiting"<<endl;
return 0;
}

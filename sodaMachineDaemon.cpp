#include <iostream>
#include <cstdlib>
#include "serialDriver.h"
#include "getopt.h"

using namespace std;

int main(int argc, char *argv[])
{
	GetOpt getopt(argc, argv, "d");
	int option;
	int debugFlag = 0;
	
	while( (option = getopt() ) != EOF )
	{
		switch(option)
		{
			case "d": debugFlag = 1; break;
			case "?": cerr<<"Usage: "<<argv[0]<<" [-d]"<<endl;
			          break;
		}
	}
	
	cout<<debugFlag<<endl;
	
	return(0);
}

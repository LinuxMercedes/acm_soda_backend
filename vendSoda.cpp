/*
	vendSoda.cpp
	Attempts to vend one soda from stack specified in parameters.
	Returns 0 upon success.
	Returns -1 on incorrect parameters.
	Returns 1 on empty stack.
*/

#include "serialDriver.h"
#include <cstdlib>

int main(int argc, char* argv[])
{
	int stackChoice = -1;
	int returnCode = 0;
	serialDriver acmSoda("/dev/ttyS0", "output");
	
	if( argc < 2 )
		exit(-1);
	else
		stackChoice = atoi(argv[1]);
	
	if( stackChoice > 7 || stackChoice < 0 )
		exit(-1);
	
	if( !acmSoda.VendCan(stackChoice) )
		returnCode = 1;
	
	return returnCode;
}

#include "utils.h"
#include <iostream>

//Size of flushbuffer, in bytes
#define FLUSHBUFFER_SIZE 1000

const char HEXTABLE[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

using namespace std;
//convert a nibble to an int
//Returns -1 on error
int HexToInt( const char input )
{
for( int i=0; i < 16; ++i)
	{
	if( HEXTABLE[i] == input )
		return i;
	}
return -1;
}

//convert two nibbles to an int
//Returns -1 on error
int HexToInt( const char msB, const char lsB )
{
if( HexToInt(msB)==-1 || HexToInt(lsB)==-1 )
	return -1;
return ( HexToInt(msB)*16 + HexToInt(lsB) );
}

//Converts two hex characters to an unsigned short
//Does not check for errors
unsigned short HexToUShort( const char msB, const char lsB)
{
return (HexToInt(msB)*16 + HexToInt(lsB) );
}

//Finds the position of the rightmost set bit
//Returns -1 on error
int FindOnBitUShort( unsigned short b )
{
for( int i=0; i<16;++i)
	{
	if( b & 0x01 )
		{
		return i;
		}
	b=(b>>1);
	}
return -1;
}


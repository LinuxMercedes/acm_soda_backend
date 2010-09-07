#include <netinet/in.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <cassert>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>

static char buf[255];

using namespace std;

#include "utils.h"
#include "serialController.h"
serialController::~serialController()
{
tcsetattr(fd,TCSANOW,&oldtio);
flock( fd, LOCK_UN );
}
#define BAUDRATE B4800

serialController::serialController( )
{
initComplete = false;
}

void serialController::Open( const string & device )
{
    fd = open(device.c_str(), O_RDWR | O_NOCTTY );
    if (fd <0) 
		{
		perror(device.c_str()); 
		exit(-1); 
		}

	if( flock( fd, LOCK_EX | LOCK_NB) != 0 )
		{
		perror(device.c_str());
		exit(-2);
		}

    tcgetattr(fd,&oldtio); /* save current port settings */
    memset( &newtio, 0x00, sizeof(newtio) );
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR | IGNBRK;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0 & !ECHO;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME]    = 0;
    newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 chars received */

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);

	initComplete = true;
}

int serialController::GetUserChoice( const unsigned int timeout)
{
int old_buf;
int new_buf;
int retn;
time_t end_time;

assert( initComplete );

end_time = time(NULL) + timeout;
retn = 1;

const char COMMAND = 'B';
retn = write( fd, &COMMAND, 1);
if( retn != 1 )
	{
	cout<<"Error writing to serial port"<<endl;
	return -1;
	}

memset( buf, 0x00, sizeof(buf) );
retn = read( fd, buf, sizeof(buf) );
if( retn != 3 )
	{
	cout<<"Error reading from serial port- read "<<retn<<"chars"<<endl;
	return -1;
	}
buf[ retn ] = 0x00;

old_buf = HexToInt( buf[1], buf[2] );
new_buf = old_buf;

while( time(NULL) < end_time && old_buf==new_buf && old_buf >= 0 && new_buf >= 0 )
	{
	retn = write( fd, &COMMAND, 1);
	if( retn != 1 )
		{
		cout<<"Error writing to serial port"<<endl;
		return -1;
		}

	memset( buf, 0x00, sizeof(buf) );
	retn = read( fd, buf, sizeof(buf) );
	if( retn != 3 )
		{
		cout<<"Error reading from serial port- read "<<retn<<"chars"<<endl;
		return -1;
		}
	buf[ retn ] = 0x00;

	new_buf = HexToInt( buf[1], buf[2] );
	}

if( old_buf == new_buf )
	{
	return -1;
	}
else if( old_buf >= 0 && new_buf >= 0 )
	{
	return FindOnBitUShort( (unsigned short)(old_buf^new_buf) );
	}
else
	{
	return -1;
	}
}

int serialController::GetCanStatus()
{
int retn = 0;

const char COMMAND = 'S';

assert( initComplete );

retn = write( fd, &COMMAND, 1);
if( retn != 1 )
	{
	cout<<"Error writing to serial port"<<endl;
	return -1;
	}

memset( buf, 0x00, sizeof(buf) );
retn = read( fd, buf, sizeof(buf) );
if( retn != 3 )
	{
	cout<<"Error reading from serial port- read "<<retn<<"chars"<<endl;
	return -1;
	}
buf[ retn ] = 0x00;

return HexToInt( buf[1], buf[2] );
}

bool serialController::VendCan(const int slot)
{
char CommandBuffer[10];

assert( initComplete );

if( slot < 0 || slot >= 8 )
	{
	cout<<"Error in vending: Invalid slot # "<<endl;
	return false;
	}

int retn = 0;

snprintf( CommandBuffer, sizeof(CommandBuffer), "V%i", slot );
//cout<<"String:"<<CommandBuffer<<endl;

retn = write( fd, CommandBuffer, strlen(CommandBuffer) );
if( retn != 2 )
	{
	cout<<"Error writing to serial port"<<endl;
	return -1;
	}

memset( buf, 0x00, sizeof(buf) );
retn = read( fd, buf, sizeof(buf) );
if( retn != 3 )
	{
	cout<<"Error reading from serial port- read "<<retn<<"chars"<<endl;
	return -1;
	}
buf[ retn ] = 0x00;
//cout<<"Got back:"<<buf<<" sleeping for 1 second"<<endl;
if( buf[ 0 ] != 'V' || buf[ 2 ] != 'Y' )
	{
	cout<<"Unknown return"<<endl;
	}

sleep(1);
//cout<<"Turning off motor, should be auto-latched"<<endl;

snprintf( CommandBuffer, sizeof(CommandBuffer), "V9" );
retn = write( fd, CommandBuffer, strlen(CommandBuffer) );
if( retn != 2 )
	{
	cout<<"Error writing to serial port"<<endl;
	return -1;
	}

memset( buf, 0x00, sizeof(buf) );
retn = read( fd, buf, sizeof(buf) );
if( retn != 3 )
	{
	cout<<"Error reading from serial port- read "<<retn<<"chars"<<endl;
	return -1;
	}
buf[ retn ] = 0x00;
//cout<<"Got back:"<<buf<<endl;

return buf[ strlen(CommandBuffer) ] == 'N';
}

#include "serialDriver.h"
#include <string>
#include <iostream>
using namespace std;
/*
class serialDriver
{
public:
    int GetCanStatus();
    bool VendCan( const int slot );
};
*/

bool serialDriver::hasSoda( const int can )
{
return ( canStatBuffer >> can ) & 1;
}

void serialDriver::Update()
{
canStatBuffer = controller.GetCanStatus();
if( stateStream )
	{
	stateStream.seekp(0,ios::beg);
	for( int i=0; i<8; ++i )
	    {
	    stateStream << ( ( canStatBuffer & ( 1 << i ) )?'X':'0' );
	    }
	stateStream<<endl;
	stateStream.flush();
	}
}

int serialDriver::GetUserChoice(const unsigned int timeout )
{
return controller.GetUserChoice( timeout );
}

int serialDriver::GetCanStatus()
{
return canStatBuffer;
}

bool serialDriver::VendCan( const int slot )
{
Update();
if( hasSoda( slot ) == false )
	{
	return false;
	}

bool retn = controller.VendCan( slot );
Update();
return retn;
}

serialDriver::serialDriver( const std::string serialPort, const std::string tempStateFile )
{
controller.Open( serialPort );

stateFile = tempStateFile;
stateStream.open( stateFile.c_str(), ios::out );

Update();
}

serialDriver::~serialDriver()
{
if( stateStream )
	{
	stateStream.close();
	unlink( stateFile.c_str() );
	stateFile = "";
	}
}

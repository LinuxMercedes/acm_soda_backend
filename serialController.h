#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <string>
#include <termios.h>

class serialController
{
public:
serialController();
~serialController();

void Open( const std::string & device );

int GetUserChoice(const unsigned int timeout);
int GetCanStatus();
bool VendCan( const int slot );
private:
bool initComplete;
int fd;
termios oldtio;
termios newtio;
};

#endif

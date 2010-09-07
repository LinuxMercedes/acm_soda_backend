#ifndef SERIALDRIVER_H
#define SERIALDRIVER_H

#include "serialController.h"
#include <string>
#include <fstream>

class serialDriver
{
public:
	serialDriver();
	serialDriver(const std::string serialPort, const std::string tempStateFile="" );
	~serialDriver();

	int GetUserChoice(const unsigned int timeout);
	int GetCanStatus();
	bool VendCan( const int slot );

private:
	void Update();
	bool hasSoda(int slot);
	int canStatBuffer;
	bool initComplete;
	serialController controller;
	std::string stateFile;
	std::fstream stateStream;
};

#endif

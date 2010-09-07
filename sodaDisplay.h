#ifndef SODADISPLAY_H
#define SODADISPLAY_H

#include <iostream>
#include <fstream>
#include <string>

class SodaDisplay {
		std::string displayFile;
		std::ofstream displayOut;
		
	public:
		SodaDisplay();
		~SodaDisplay();
		SodaDisplay(string displayFile);
		
		string getDisplayFile();
		void setDisplayFile(string newDisplayFile);
		bool writeLine(int lineNum, string text);
};

#endif

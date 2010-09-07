#include "sodaDisplay.h"

using namespace std;

SodaDisplay::SodaDisplay() {
	displayFile = "null";
}

SodaDisplay::SodaDisplay(string displayFile) {
	this.displayFile = displayFile;
}

string SodaDisplay::getDisplayFile() {
	return(displayFile);
}

void SodaDisplay::setDisplayFile(string newDisplayFile) {
	displayFile = newDisplayFile;
}

bool SodaDisplay::writeLine(int lineNum, string text) {
	return(true);
}

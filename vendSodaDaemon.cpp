#include <fstream>
#include <cstdlib>
#include "serialDriver.h"
#include <sys/stat.h>

#define PIPE_IN_NAME "/tmp/vendsodain"
#define PIPE_OUT_NAME "/tmp/vendsodaout"

using namespace std;

int main()
{
	char slotChoice[256];
	fstream vendPipeIn;
	fstream vendPipeOut;
	int processID;
	bool vendSuccess;
	
	processID = fork();
	
	// Spawn off the daemon process and exit
	if(processID)
	{
		printf("Spawned process %i\n", processID);
		return 0;
	}
	
	// Making pipes...
	if( mkfifo(PIPE_IN_NAME, S_IRWXU) == -1 )
	{
		fprintf(stderr, "Error creating pipe '%s'\n", PIPE_IN_NAME);
		exit(-1);
	}
	chmod(PIPE_IN_NAME, S_IRWXU|S_IWOTH);

	if( mkfifo(PIPE_OUT_NAME, S_IRWXU) == -1 )
	{
		fprintf(stderr, "Error creating pipe '%s'\n", PIPE_OUT_NAME);
		exit(-1);
	}
	chmod(PIPE_OUT_NAME, S_IRWXU|S_IROTH);
	
	// Make the sodamachine object
	serialDriver acmSoda("/dev/ttyS0", "output");
	
	while(1)
	{
		vendPipeIn.open(PIPE_IN_NAME, fstream::in);
		
		vendPipeIn.getline(slotChoice, 256);
		
		vendSuccess = acmSoda.VendCan(atoi(slotChoice));
		
		//printf("Vended soda %i\n", atoi(slotChoice) );
		
		vendPipeIn.close();
		
		vendPipeOut.open(PIPE_OUT_NAME, fstream::out);
		
		vendPipeOut<<vendSuccess;
		
		vendPipeOut.close();
	}
	return 0;
}

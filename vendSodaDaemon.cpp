#include <fstream>
#include <cstdlib>
#include "serialDriver.h"
#include <sys/stat.h>

#define PIPE_IN_NAME "/tmp/vendsodain"
#define PIPE_OUT_NAME "/tmp/vendsodaout"

#define DEBUG 1

#ifdef DEBUG
#define DEBUG_LOG "/tmp/vendsodalog"
#include <time.h>
#endif

using namespace std;

#ifdef DEBUG
string getTime()
{
    time_t *rawtime;
    struct tm *timeinfo;
    time (rawtime);
    timeinfo = localtime(rawtime);

    return asctime(timeinfo);
}
#endif

int main()
{
	char slotChoice[256];
	fstream vendPipeIn;
  fstream vendPipeOut;

#ifdef DEBUG
  fstream vendLog;
#endif

  int processID;
	bool vendSuccess;
	
	processID = fork();
	
	// Spawn off the daemon process and exit
	if(processID)
	{
		printf("Spawned process %i\n", processID);
		return 0;
	}

  // Clean up old pipes if they exist
  if(remove(PIPE_IN_NAME))
  {
    printf("Error deleting pipe '%s'\n", PIPE_IN_NAME);
  }
  else
  {
    printf("Sucessfully deleted pipe '%s'\n", PIPE_IN_NAME);
  }

  if(remove(PIPE_OUT_NAME))
  {
    printf("Error deleting pipe '%s'\n", PIPE_OUT_NAME);
  }
  else
  {
    printf("Sucessfully deleted pipe '%s'\n", PIPE_OUT_NAME);
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

#ifdef DEBUG
  vendLog.open(DEBUG_LOG, fstream::out);
  vendLog << "\n\nOpened logging at " << getTime() << "\n";
  vendLog.close();
#endif

	while(1)
	{
#ifdef DEBUG
    vendLog.open(DEBUG_LOG, fstream::out);
#endif

    vendPipeIn.open(PIPE_IN_NAME, fstream::in);
		
		vendPipeIn.getline(slotChoice, 256);

#ifdef DEBUG
    vendLog << getTime() << " " << slotChoice << "\n";
    
#endif  

    vendSuccess = acmSoda.VendCan(atoi(slotChoice));
		
		//printf("Vended soda %i\n", atoi(slotChoice) );
		
		vendPipeIn.close();
		
		vendPipeOut.open(PIPE_OUT_NAME, fstream::out);
		
		vendPipeOut<<vendSuccess;
#ifdef DEBUG
    vendLog << vendSuccess << "\n\n";
    vendLog.close();
#endif

		vendPipeOut.close();
	}
	return 0;
}

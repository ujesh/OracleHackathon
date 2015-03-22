// SlaveProcess.cpp : Defines the entry point for the console application.
//


#include <iostream>
#include "Slave.h"

#define EXIT_FAILURE 1
int gi_slaveProcessID = 0;


int main(int argc, char* argv[])
{
	int li_argCount = 2;

	if ( li_argCount > argc ) {
		std::cout<<"Invalid Number of arguments: Usage SlaveProcess <Slave Process ID>";
		return EXIT_FAILURE;
	}
	else
	{
		if(strlen(argv[1]) > 0 )
		{
			gi_slaveProcessID = atoi(argv[1]);
		}
		if(gi_slaveProcessID == 0)
		{
			std::cout<<"Invalid Slave Process ID"<<std::endl;
		}
	}

	SlaveData *lup_slaveData = new SlaveData;
	lup_slaveData->SetID(gi_slaveProcessID);
	Slave	*lup_slave = new Slave;

	lup_slave->StartThreads();

	while(1)
	{
		Sleep(1000);
	}

	getchar();
	return 0;
}



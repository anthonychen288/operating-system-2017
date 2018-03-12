#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	
	char cpulimit[100];
	int limit, time;
	// handling input
	sscanf(argv[1], "%d", &limit);
	sscanf(argv[2], "%d", &time);
	printf("%d, %d\n", limit, time);
	
	int cpid;
	cpid = fork();
	
	if(cpid < 0)	{
		printf("fork error. try again\n");
		return -1;
	}else if(cpid == 0)	{	// child process
		while(1);
	}else	{				// parent process
	
		sprintf(cpulimit, "cpulimit -l %d -p %d &", limit, cpid);
		printf("%s\n", cpulimit);
		system(cpulimit);
		printf("sleep...\n");
		sleep(time);
		printf("wakeup and kill\n");
		kill(cpid, SIGKILL);
	}
	return 0;
}
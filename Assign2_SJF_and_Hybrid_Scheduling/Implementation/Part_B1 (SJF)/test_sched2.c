
#include "user.h"
#define stdout 1
#define stderr 2

// delay function adds a delay of approx n seconds
void delay(int n)
{
	int i, j, k;
	for (i = 0; i < n; i++)
		for (j = 0; j < 1000; j++)
			for (k = 0; k < 10000; k++)
				asm("nop"); // assembly instruction to do nothing
}

int main()
{
	// array containing burst times
	int t[10] = {5, 1, 15, 10, 15, 30, 30, 20, 60, 50};      // list of burst time of processes
    int cpu_io[10] = {0, 1, 1, 0, 0, 1, 1, 0, 1, 1};            // 0 indicates cpu bound process, 1 indicates io bound process

	long x = 0;
	set_burst_time(1);

	for (int i = 0; i < 10; i++)
	{
		// if child process, then set burst time, perform computation and exit
		if (fork() == 0)
		{
			x = set_burst_time(t[i]);

			if (x < 0)
			{
				printf(stderr, "Couldn't set burst time for process %d\n", getpid());
			}

			// code to add delay
			delay(t[i]);

			if (cpu_io[i] == 0)
				printf(stdout, "CPU Bound process (child number: %d)\n", i);

			// IO bound process
			else
			{
				// mimicking IO wait
				for (int i2 = 0; i2 < t[i] * 5; i2++)
				{
					sleep(1);
				}
				printf(stdout, "IO Bound process (child number: %d with %d IO Requests)\n", i, t[i] * 5);
			}
			exit();
		}
	}

	while (wait() != -1);
	exit();
}

#include "user.h"
#define stdout 1

int main()
{
    printf(stdout, "The total number of processes are: %d\n", getNumProc());
    printf(stdout, "Maximum Process ID: %d\n", getMaxPid());                    // make better test cases
    int curProcID = getpid();
    printf(stdout, "This process has process ID: %d\n", curProcID);

    struct processInfo *input = (struct processInfo* )malloc(sizeof(struct processInfo));
    input->ppid = 0;
    input->psize = 0;
    input->numberContextSwitches = 0;
    printf(stdout, "Getting Process Information...\n");
    getProcInfo(curProcID, input);
    printf(stdout, "Information Retrieval Complete\n");
    printf(stdout, "Parent ID: %d\t Size: %d\nNumber of times this process was context switched in: %d\n", input->ppid, input->psize, input->numberContextSwitches);

    printf(stdout, "Burst time of this process (pid = %d) initially (should be 0): %d\n", curProcID, get_burst_time());
    set_burst_time(14);
    printf(stdout, "Burst time of this process (pid = %d) set to (should be 14): %d\n", curProcID, get_burst_time());
    set_burst_time(20);
    printf(stdout, "Burst time of this process (pid = %d) set to (should be 20): %d\n", curProcID, get_burst_time());
    exit();
}

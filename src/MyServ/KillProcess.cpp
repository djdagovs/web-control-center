//============================================================================
// Name        : KillProcess.cpp
// Author      : MioChen
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>

#include <stdlib.h>
#include <fcntl.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

unsigned int getProcessID(char *p_processname)
{
    DIR *dir_p;
    struct dirent *dir_entry_p;
    char dir_name[40];                                        // ??? buffer overrun potential
    char target_name[252];                                    // ??? buffer overrun potential
    int target_result;
    char exe_link[252];
    //int errorcount = 0;
    int result = -1;

    dir_p = opendir("/proc/");

    printf("Search for");
    printf(target_name);
    printf("/n<br>");

    while(NULL != (dir_entry_p = readdir(dir_p))) {                                            // Reading /proc/ entries

        if (strspn(dir_entry_p->d_name, "0123456789") == strlen(dir_entry_p->d_name)) {        // Checking for numbered directories
            strcpy(dir_name, "/proc/");
            strcat(dir_name, dir_entry_p->d_name);
            strcat(dir_name, "/");                                                             // Obtaining the full-path eg: /proc/24657/
            exe_link[0] = 0;
            strcat(exe_link, dir_name);
            strcat(exe_link, "exe");                                                         // Getting the full-path of that exe link
            target_result = readlink(exe_link, target_name, sizeof(target_name)-1);            // Getting the target of the exe ie to which binary it points to
            if (target_result > 0) {
                target_name[target_result] = 0;
                printf(target_name);
                printf("/n<br>");
                if (strstr(target_name, p_processname) != NULL) {                            // Searching for process name in the target name -- ??? could be a better search !!!
                    result = atoi(dir_entry_p->d_name);
                    //printf("getProcessID(%s) :Found. id = %d\n", p_processname, result);
                    closedir(dir_p);
                    return result;
                }
            }
        }
    }
    printf("/n<br>");
    closedir(dir_p);
    return result;
}

int main(int argc, char* argv[]) {
	if(argc==2)
	{
		int nPid = getProcessID(argv[1]);


		if(nPid>0)
		{
			kill(nPid,SIGTERM);
			printf("1");
		}
		else
			printf("0");
		return 0;
	}
	return 0;
}


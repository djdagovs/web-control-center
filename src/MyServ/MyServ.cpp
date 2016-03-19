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
#include "MyServ.h"

#define LOCKFILE "/home/pi/WCC_daemon.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)


char fifoPath[255]="/home/pi/WCC_fifo_server";
char XMLPath[255]="/usr/bin/web-control-center/cmd_map.xml";

bool m_bLEDStatus = false;
bool m_bEnableLED = true;


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

    dir_p = opendir("/proc/");                                                                 // Open /proc/ directory
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
                if (strstr(target_name, p_processname) != NULL) {                            // Searching for process name in the target name -- ??? could be a better search !!!
                    result = atoi(dir_entry_p->d_name);
                    //printf("getProcessID(%s) :Found. id = %d\n", p_processname, result);
                    closedir(dir_p);
                    return result;
                }
            }
        }
    }
    closedir(dir_p);
    return result;
}


extern int lockfile(int);
int filelock(int fd){ struct flock fl; fl.l_type = F_WRLCK; /* F_RDLCK, F_WRLCK(an exclusive write lock), or F_UNLCK(unlocking a region) */ fl.l_start = 0; /* offset in bytes relative to l_whence */ fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK _CUR, SEEK _END */ fl.l_len = 0; /* means lock to EOF */ /*fcntl function can change the properties of file that is already open * Here F_SETLK set the record locks define in flock structure var */ return(fcntl(fd, F_SETLK, &fl)); }

int already_running()
{
    int     fd;
    int     ret;
    char    buf[16];

    fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
    if (fd < 0) {
    	printf("check single fail\n");
        syslog(LOG_ERR, "can't open %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }
    if (filelock(fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            return(1);
        }
        printf("check single fail\n");
        syslog(LOG_ERR, "can't lock %s: %s", LOCKFILE, strerror(errno));
        exit(1);
    }

    ret = ftruncate(fd, 0);
    ret = sprintf(buf, "%ld", (long)getpid());
    ret = write(fd, buf, strlen(buf)+1);

    return(0);
}


int daemon_init(void)
{
	syslog(LOG_ERR, "MyServ stare:daemon_init");
	pid_t pid;
	if((pid = fork()) < 0)
	{
		syslog(LOG_ERR, "MyServ stare:fork fail");
		return(-1);
	}
	else if(pid != 0)
        {
		syslog(LOG_ERR, "MyServ stare:fork success but pid!=0-exit");
		exit(0); /* parent exit */
	}
	/* child continues */
        syslog(LOG_ERR, "MyServ stare:fork setsid");
	setsid(); /* become session leader */
        syslog(LOG_ERR, "MyServ stare:fork setsid ok");
	//chdir("//"); /* change working directory */
	umask(0); /* clear file mode creation mask */
	syslog(LOG_ERR, "MyServ stare:umask ok");

	close(0); /* close stdin */
	syslog(LOG_ERR, "MyServ stare:fork close ok");

	//close(1); /* close stdout */
	//close(2); /* close stderr */
	return(0);
}
void sig_term(int signo)
{
	if(signo == SIGTERM)
	{
		ChangeLED(false);
		printf("Service Stoped\n");
		syslog(LOG_INFO, "program terminated.");
		closelog();
		exit(0);
	}
}

void CreatePipe()
{
	int file1,file2;
	int fd;
	char str[256];
	char temp[4]="how";
	char temp1[4];
	file1 = mkfifo(fifoPath,0666);
	if(file1<0) {
		printf("Unable to create a fifo");
		return;
	}
	close(file1);
}

void ProcessFIFO(int fifo_server)
{
	//printf("ProcessFIFO \n");
	int choice;
        char szCMD[256];
	strcpy(szCMD,"");

	if(fifo_server==-1)
	{
		printf("Error opening file");
		return;
	}

	//printf("FIFO read \n");
	char szCMDTemp[256]="";
	char szCMDName[256]="";
	char szCMDParam[256]="";
	strcpy(szCMD,"");
	int nSize = read(fifo_server,szCMDTemp,sizeof(char)*256);
	strncpy(szCMD,szCMDTemp,nSize);
	if(nSize<=0)
		return;

	printf("[CMD Receive ]:%s \n",szCMD);

	int nSepIndex = -1;
	for(int i = 0;i<nSize;i++)
	{
		if(szCMD[i]=='@')
		{
			nSepIndex = i;
			break;	
		}
	}

	printf("[CMD Receive @ index]:%d \n",nSepIndex);
	if(nSepIndex>0)
	{
		strncpy(szCMDName,szCMD,nSepIndex);
		strncpy(szCMDParam,szCMD+nSepIndex+1,nSize-nSepIndex-1);
	}
	else
		strncpy(szCMDName,szCMD,nSize);

	printf("[CMD Receive Name]:%s \n",szCMDName);
	printf("[CMD Receive Param]:%s \n",szCMDParam);

	CMD_OBJ* pCMD_OBJ = m_pCMD_Map->GetCommand(szCMDName);

	
	if(pCMD_OBJ)
	{
		printf("[CMD Found ]:%s \n",pCMD_OBJ->szCmd);

		if(nSepIndex>0)
		{
			char szCMDFinal[256]="";
			sprintf(szCMDFinal,"%s %s",pCMD_OBJ->szCmd,szCMDParam);
			system((char*)szCMDFinal);
			printf("[CMD Found ]:%s \n",szCMDFinal);
		}
		else
		{
			system((char*)pCMD_OBJ->szCmd);
		}
	}
	else
	{
		printf("[CMD Not Found]:%s \n",szCMD);
	}
	/*
	//printf("FIFO read ok :%d\n",choice);
	switch(choice)	{
		case 1:
			ChangeLED(true);
			printf("\nData get 1\n");
		break;

		case 2:
			ChangeLED(false);
			printf("\nData get 2\n");
		break;
		case 3:
			StartPlayRadio();
			printf("\nData get 3\n");
		break;
		case 4:
			StopPlayRadio();
		break;
	}
	*/
//	printf("\nCMD get->> %s \n",szCMD);
}
void printHelp()
{
	printf("[Help] \n");
	printf("-start start service \n");
	printf("-stop  stop  service \n");
	printf("-send  send cmd ex: send 1\n");
}

void ServiceStart()
{

	/*
	if(already_running()==1)
	{
			printf("Service already exist\n");
			return;
	}
	*/
	if(daemon_init() == -1)
	{
		printf("can't fork self\n"); 
		exit(0);
	}
	else
	{
		m_pCMD_Map = new CMD_Map();
	        m_pCMD_Map->Load(XMLPath);
	        //printf("CreatePipe...\n");
		CreatePipe();

	        //printf("InitLED...\n");
		//InitLED();
		//printf("ChangeLED...\n");	
		//ChangeLED(true);
	        //printf("service started...\n");
	}

	openlog("daemontest", LOG_PID, LOG_USER);
	syslog(LOG_INFO, "program started.");
	signal(SIGTERM, sig_term); /* arrange to catch the signal */
	//system("ffplay -nodisp mmsh://bcr.media.hinet.net/RA000042");
	int fifo_server;
	fifo_server = open(fifoPath,O_RDONLY);

	printf("open fifo =>%d\n",fifo_server);
	
	/*
	printf("m_pCMD_Map->GetCommand test\n");
	CMD_OBJ* pCMD_OBJ = m_pCMD_Map->GetCommand("name3");
	
	if(pCMD_OBJ!=NULL)
	{
	printf("Find %s\n",pCMD_OBJ->szID);
	printf("get %s\n",pCMD_OBJ->szCmd);
		
	}
	*/
	if(fifo_server==-1)
		printf("open fifo fail\n");
	else
		printf("open fifo Success\n");

        if(fifo_server!=-1)
	{
		while(1)
		{
		//printf("Radio run\n");
		//printf("read pipe\n");

		//m_bLEDStatus = !m_bLEDStatus;
		//ChangeLED(m_bLEDStatus);

		if(fifo_server>=0)
			ProcessFIFO(fifo_server);
		sleep(1);
		//printf("read pipe end\n");
		//else
			//	break;
			//else
		 	//printf("open fifo fail\n");
		 //sleep(1); /* put your main program here */
		}
	}
	if(m_pCMD_Map)
	{
		delete m_pCMD_Map;
		m_pCMD_Map = NULL;
	}
	if(fifo_server!=-1)
	close(fifo_server);
}
extern char *__progname;

void InitLED()
{
	/*
	if(m_bEnableLED == false)
			return;
	int nRet = system("gpio mode 0 out");
	*/
}

void ChangeLED(bool bShow)
{
	/*
	if(m_bEnableLED == false)
		return;
	int nRet = 0;
	if(bShow)
		nRet = system("gpio write 0 0");
	else
		nRet = system("gpio write 0 1");
	*/
}


void StartPlayRadio()
{
        int pid = getProcessID("ffplay");
        //printf("id=%d",pid);
        if(pid==-1)
                system("ffplay -nodisp mmsh://bcr.media.hinet.net/RA000042 &");
}

void StopPlayRadio()
{
	int pid = getProcessID("ffplay");
        //printf("id=%d",pid);
	if(pid!=-1)
		kill(pid,SIGTERM);
}

void ServiceStop()
{
	printf("call ServiceStop\n");

	printf("Search for %s\n",__progname);

	pid_t pidSelf;
	pidSelf = getpid();


	int pid = -1;
	pid = getProcessID(__progname);
	printf("self pid=%d\n",pidSelf);
	printf("pid=%d\n",pid);

	if(pid!=-1 && pid!=0)
	{
		printf("call kill\n");
		kill(pid,SIGTERM);

	}
	
	printf("call ServiceStop end\n");
}

void ServiceSend(char* chCommandID)
{
	int fifo_server;
	char szCMD[256];
	strcpy(szCMD,"");
	
	int nCmdLen = strlen(chCommandID);
	strncpy(szCMD,chCommandID,nCmdLen);
	szCMD[nCmdLen]='\0';
	//fifo_server = open("/home/pi/web/MyServ/fifo_server",O_RDWR);
	fifo_server = open(fifoPath,O_RDWR);
	//printf("open end=\n");
	int nCmdIdLen = strlen(chCommandID);
	if(fifo_server>=0)
	{
		write(fifo_server,szCMD,256);
		close(fifo_server);
		//printf("send cmd ok: %s\n",szCMD);
	}
	else
	{
		printf("send cmd fail\n");
	}
}

int main(int argc, char* argv[])
{
	if(argc==1)
	{
		printHelp();
		return 0;
	}

	if(strcmp(argv[1],"start")==0)
	{
		ServiceStart();
		return 0;
	}
	else if(strcmp(argv[1],"stop")==0)
	{
		printf("call ServiceStop from Main!!!\n");
		ServiceStop();
		return 0;
	}
	else if(strcmp(argv[1],"send")==0)
	{
		ServiceSend(argv[2]);
	/*
		if(strcmp(argv[2],"1")==0)
			ServiceSend("1");
		else if(strcmp(argv[2],"2")==0)
			ServiceSend("2");
		else if(strcmp(argv[2],"3")==0)
                        ServiceSend("3");
		else if(strcmp(argv[2],"4")==0)
                        ServiceSend("4");
	*/
		return 0;
	}


	printf("argument invalid\n");
	return 0;

}

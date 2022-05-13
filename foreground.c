#include "headers.h"

void foreground(char *command)
{
    char buf1[MAX_SIZE];
    strcpy(buf1,command);
    int status;
    // generate the arg vector for execvp
    char *com = strtok(buf1," ");
    char *args[MAX_SIZE];
    int i = 0;
    while(com != NULL)
    {
        args[i++] = com;
        com = strtok(NULL," ");
    }
    args[i] = NULL;
    //int controlterm = open("/dev/tty",O_RDWR);
   
    signal(SIGTTOU,SIG_IGN);
    //signal(SIGTTIN,SIG_IGN);
    // fork to create a child and wait for the parent process
    pid_t pid = fork();
    if(pid == 0)
    {   
        int childpid = getpid();
        setpgid(0,0);
        tcsetpgrp(STDIN_FILENO,childpid);
        if(execvp(args[0],args)<0)
        {
            printf("Invalid Command\n");
            exit(1);
        }
    }
    else if(pid == -1)
    {
        perror("Could not Execute\n");
        return;
    }
    else
    {
        
        foreproc->pid = pid;
        strcpy(foreproc->procname,args[0]);
        
        waitpid(pid, &status, WUNTRACED);

        if(WIFSTOPPED(status))
        {
            jh[jobidx].jno = jobidx+1;
            jh[jobidx].pid = pid;
            strcpy(jh[jobidx].jobname,args[0]);
            jh[jobidx].state = 'T';
            jobidx++;
        }

        tcsetpgrp(STDIN_FILENO,shellpid);
        
        foreproc->pid = shellpid;
        strcpy(foreproc->procname,"SHELL");
        //printf("%d %d %s\n",jobidx,jh[jobidx].pid,jh[jobidx].jobname);
        return;
    }
}
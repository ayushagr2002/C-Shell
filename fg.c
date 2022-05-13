#include "headers.h"

void fg(char *command)
{
    char buf[MAX_SIZE];
    strcpy(buf,command);
    char *saveptr;
    char *temp = strtok_r(buf," ",&saveptr);
    char *jn = strtok_r(NULL," ",&saveptr);
    if(jn == NULL)
    {
        printf("Please Enter job number\n");
        return;
    }
    int jobno = atoi(jn);
    int pos = 0;
    int i=0;
    while(i <= jobidx && jh[i].jno != jobno)
    {
        i++;
        pos = i;
    }
    if(jh[pos].jno != jobno)
    {
        printf("No job with job number %d exists\n",jobno);
        return;
    }
    int pid = jh[pos].pid;
    foreproc->pid = pid;
    strcpy(foreproc->procname,jh[pos].jobname);
    
    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);

    tcsetpgrp(STDIN_FILENO,pid);
    kill(pid,18);
    
    int status;
    waitpid(pid,&status,WUNTRACED);

    if(WIFSTOPPED(status))
    {
        jh[pos].state = 'T';
    }
    else
        jh[pos].jno = 0;

    tcsetpgrp(STDIN_FILENO,shellpid);
    foreproc->pid = shellpid;
    strcpy(foreproc->procname,"SHELL");
    return;
}
#include "headers.h"

void bg(char *command)
{
    char buf[MAX_SIZE];
    strcpy(buf,command);
    char *saveptr;
    char *temp = strtok_r(buf," ",&saveptr);
    char *jn = strtok_r(NULL," ",&saveptr);
    int jobno = atoi(jn);
    //printf("%d\n",jobno);
    int i = 0;
    while(i <= jobidx && jh[i].jno != jobno)
    {
        i++;
    }
    if(jh[i].jno != jobno)
    {
        printf("No job with job number %d exists\n",jobno);
        return;
    }
    int pid = jh[i].pid;
    //printf("%d %s\n",pid,jh[i].jobname);
    if(jh[i].state == 'R')
        return;
    //printf("Got process\n");
    //tcsetpgrp(STDIN_FILENO,shellpid);
    setpgid(pid,0);
    kill(pid,SIGCONT);
    //tcsetpgrp(STDIN_FILENO,shellpid);
    //setpgid(pid,pid);
    jh[i].state = 'R';
    return;
}
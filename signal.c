#include "headers.h"

void sendsignal(char *command)
{
    char buf[MAX_SIZE];
    strcpy(buf,command);
    char *saveptr;
    char *temp = strtok_r(buf," ",&saveptr);
    char *jn = strtok_r(NULL," ",&saveptr);
    char *sign = strtok_r(NULL," ",&saveptr);
    int jobid = atoi(jn);
    int signo = atoi(sign);
    int pid;
    int i=0;
    while(i < jobidx && jh[i].jno != jobid)
    {
        i++;
    }
    if(jh[i].jno == 0)
    {
        printf("No job with job number %d exists\n",jobid);
        return;
    }
    if(kill(jh[i].pid,signo)<0)
    {
        printf("Could not send signal\n");
        return;
    }
    return;
}
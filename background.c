#include "headers.h"

int gethash(int n)
{
    int hash_value = 0;
    while(n%10)
    {
        hash_value += n%10;
        n = n/10;
    }
    return hash_value;
}


void background(char *command)
{
    char buffer[MAX_SIZE];
    strcpy(buffer,command);
    char *args[MAX_SIZE];
    int i = 0;
    char *cmd = strtok(buffer," ");
    
    while(strcmp(cmd,"&") != 0)
    {
        args[i++] = cmd;
        cmd = strtok(NULL," ");
    }
    args[i] = NULL;

    pid_t pid = fork();
    //setpgid(0,0);
    if(pid == 0)
    {
        setpgid(0,0);
        if(execvp(args[0],args) < 0)
        {
            printf("Invalid Command\n");
            exit(1);
        }
    }
    else if(pid == -1)
    {
        perror("Could not execute");
        return;
    }
    else
    {
        //tcsetpgrp(STDIN_FILENO,shellpid);
        printf("%d\n",pid);

        jh[jobidx].jno = jobidx + 1;
        jh[jobidx].pid = pid;
        jh[jobidx].state = 'R';
        strcpy(jh[jobidx].jobname,args[0]);
        i=1;
        while(args[i] != NULL)
        {
            strcat(jh[jobidx].jobname," ");
            strcat(jh[jobidx].jobname,args[i]);
            i++;
        }
        jobidx++;
        return;
    }
}
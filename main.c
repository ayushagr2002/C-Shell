#include "headers.h"

void handler1(int signal)
{
    int olderrno = errno;
    int pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED)) > 0)
    {
        // int procindex = gethash(pid);
        int i = 0;
        int pos = 0;
        for(i=0;i<MAX_SIZE;i++)
        {
            if(jh[i].pid == pid)
            {
                pos = i;
                break;
            }
                
        }
        if(WIFSTOPPED(status))
        {
            jh[pos].state = 'T';
            //printf("%d\n",pos);
        }
        else if(WIFCONTINUED(status))
        {
            jh[pos].state = 'R';
        }
        else if (WIFEXITED(status))
        {
            fprintf(stderr, "\n%s with pid %d exited normally\n", jh[pos].jobname, pid);
            jh[pos].jno = 0;
            prompt();
            fflush(stdout);
        }
        else
        {
            fprintf(stderr, "\n%s with pid %d exited abnormally\n",jh[pos].jobname, pid);
            jh[pos].jno = 0;
            prompt();
            fflush(stdout);
        }

    }
}

void handler2(int signo)
{
    if(foreproc->pid != shellpid)
    {
        kill(foreproc->pid,SIGTSTP);
        printf("\n");
    }
    else
    {
        printf("\n");
        prompt();
        fflush(stdout);
    }
    //signal(SIGTSTP,handler2);
    return;
}

void handler3(int signo)
{
    if(foreproc->pid != shellpid)
    {
        kill(foreproc->pid,SIGINT);
        tcsetpgrp(STDIN_FILENO,shellpid);
        return;
    }
    else
    {
        printf("\n");
        prompt();
        fflush(stdout);
        return;
    }
}

int main()
{
    size_t size = 10;
    char *commands = (char *)malloc(sizeof(char) * size);
    getcwd(home, MAX_SIZE);
    signal(SIGCHLD, handler1);
    signal(SIGTSTP,handler2);
    signal(SIGINT,handler3);

    for (int i = 0; i < 200; i++)
    {
        bgp[i][0] = '\0';
    }
    lastcommand[0] = '\0';
    strcpy(path,home);
    strcat(path,"/history.txt");
    
    jh = (struct jobholder *)malloc(sizeof(struct jobholder)*MAX_SIZE);
    foreproc = (struct foregroundproc *)malloc(sizeof(struct foregroundproc));
    shellpid = getpid();
    foreproc->pid = shellpid;
    strcpy(foreproc->procname,"SHELL");
    jobidx = 0;
    bgproc = 0;
    foregroundprocpid = shellpid;
    setpgid(0,0);
    while (1)
    {
        prompt();
        int len;
        if((len = getline(&commands, &size, stdin))<0)
        {
            //perror("");
            printf("Bye\n");
            exit(1);
        }
        commands[len - 1] = '\0';
        store(commands,path);
        if (strlen(commands) > 0)
        {
            char *saveptr;
            char *comm = strtok_r(commands, ";", &saveptr);
            while (comm != NULL)
            {
                checkcommand(comm);
                comm = strtok_r(NULL, ";", &saveptr);
            }
        }
    }
    free(jh);
}

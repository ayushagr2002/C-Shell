#include "headers.h"

void checkcommand(char *command)
{
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stdin = dup(STDIN_FILENO);
    int redir = checkredir(command);
    int pip = checkpipe(command);
    if(pip)
    {
        pipeit(command);
    }
    else if(redir)
    {
        //printf("GOTT PIPE\n");
        //printf("%d\n",pip);
        redirec(command);
    }
    else
    {
        processcommands(command);
    }
    dup2(saved_stdin,STDIN_FILENO);
    dup2(saved_stdout,STDOUT_FILENO);
    return;
}

void processcommands(char *command){

    char buffer[MAX_SIZE];
    char buf2[MAX_SIZE];
    if(command == NULL)
        return;
    strcpy(buffer,command);
    strcpy(buf2,command);
    char *saveptr;
    char *part1;
    
    part1 = strtok(buffer," ");
    char *part2 = strtok(NULL," ");
    while(part2 != NULL)
    {
        if(strcmp(part2,"&") == 0)
        {
            background(command);
            return;
        }
        part2 = strtok(NULL," ");
        //printf("%s\n",part2);
    }

    if(strcmp(part1,"cd") == 0)
        cd(command);
    else if(strcmp(part1,"echo") == 0)
        echo(command);
    else if(strcmp(part1,"pwd") == 0)
        pwd();
    else if(strcmp(part1,"repeat") == 0)
        repeat(command);
    else if(strcmp(part1,"ls") == 0)
        ls(command);
    else if(strcmp(part1,"pinfo") == 0)
        pinfo(command);
    else if(strcmp(part1,"history") == 0)
        gethistory(command,path);
    else if(strcmp(part1,"jobs") == 0)
        jobs(command);
    else if(strcmp(part1,"sig") == 0)
        sendsignal(command);
    else if(strcmp(part1,"fg") == 0)
        fg(command);
    else if(strcmp(part1,"bg") == 0)
        bg(command);
    else if(strcmp(part1,"replay") == 0)
        replay(command);
    else if(strcmp(part1,"exit") == 0)
        exit(0);
    else
        foreground(command);
    return;
}
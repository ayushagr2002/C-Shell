#include "headers.h"

void cd(char *command)
{
    char buf1[MAX_SIZE];
    strcpy(buf1,command);
    char tempdir[MAX_SIZE];
    char *temp = strtok(buf1," ");
    char *arg = strtok(NULL," ");
    // calculate arguments, if > 1 throw an error
    int arguments = 0;
    while(temp != NULL)
    {
        temp = strtok(NULL," ");
        arguments++;
    }
    if(arguments > 1)
    {
        printf("cd:Error,Too many arguments provided.\n");
        return;
    }
    // if arg is NULL or ~, then go to home directory
    if(arg == NULL)
    {
        getcwd(prevdir,MAX_SIZE);
        chdir(home);
        return;
    }
    if(strcmp(arg,"~") == 0)
    {
        getcwd(prevdir, MAX_SIZE);
        chdir(home);
        return;
    }
    // if arg is -, go to prev directory
    if(strcmp(arg,"-") == 0)
    {
        strcpy(tempdir,prevdir);
        getcwd(prevdir,MAX_SIZE);
        chdir(tempdir);
        return;
    }
    getcwd(prevdir, MAX_SIZE);
    // if cant change directory, throw error and exit
    if(chdir(arg) < 0)
    {
        perror("Cannot change directory");
        return;
    }
}
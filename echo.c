#include "headers.h"

void echo(char *command)
{
    char buf1[MAX_SIZE];
    strcpy(buf1,command);
    char *saveptr;
    char *temp = strtok_r(buf1," ",&saveptr);
    char *arg = strtok_r(NULL," ",&saveptr);
    while(arg != NULL)
    {
        printf("%s ",arg);
        arg = strtok_r(NULL," ",&saveptr);
    }
    printf("\n");
    return;
}
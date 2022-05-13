#include "headers.h"

void repeat(char *command)
{
    char command2[MAX_SIZE];
    char command3[MAX_SIZE];
    strcpy(command2,command);
    
    char *temp = strtok(command2," ");
    char *n = strtok(NULL," ");
    temp = strtok(NULL," ");
    command3[0] = '\0';
    int size = atoi(n);

    while(temp != NULL)
    {
        strcat(command3,temp);
        int n1 = strlen(command3);
        command3[n1] = ' ';
        command3[n1+1] = '\0';
        temp = strtok(NULL," ");
    }
    int n2 = strlen(command3);
    command3[n2] = '\0';
    
    for(int i=0;i<size;i++)
    {
        processcommands(command3);
    }
    return;
}
#include "headers.h"

void replay(char *command)
{
    char buf[MAX_SIZE];
    strcpy(buf,command);
    char *saveptr;
    char *arg = strtok_r(buf," ",&saveptr);
    arg = strtok_r(NULL," ",&saveptr);
    char commandexec[MAX_SIZE];
    commandexec[0] = '\0';
    if(strcmp(arg,"-command") == 0)
    {
        arg = strtok_r(NULL," ",&saveptr);
        
        while(strcmp(arg,"-interval") != 0)
        {
            strcat(commandexec,arg);
            strcat(commandexec," ");
            arg = strtok_r(NULL," ",&saveptr);
        }
        arg = strtok_r(NULL," ",&saveptr);
        int interval = atoi(arg);
        arg = strtok_r(NULL," ",&saveptr);
        arg = strtok_r(NULL," ",&saveptr);
        int period = atoi(arg);
        int t= interval;
        while(t <= period)
        {
            sleep(interval);
            processcommands(commandexec);
            t += interval;
        }
        t -= interval;
        if(period - t > 0)
            sleep(period-t);
    }
    return;
}
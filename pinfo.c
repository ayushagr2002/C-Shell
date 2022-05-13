#include "headers.h"

void pinfo(char *command)
{
    char buf1[MAX_SIZE];
    int pid;
    strcpy(buf1,command);
    char *temp = strtok(buf1," ");
    char *piduser = strtok(NULL," ");
    if(piduser)
        pid = atoi(piduser);
    else
        pid = getpid();
    char buf[500];
    char *p;
    char state;
    long long vmemory;
    sprintf(buf, "/proc/%d/stat", pid);

    FILE *procf = fopen(buf, "r");
    if(procf == NULL)
    {
        perror("Could not find process");
        return;
    }
    char info[MAX_SIZE];
    int size = 200;
    int i = 0;
    int tpgid;
    while (fscanf(procf, "%s", info))
    {
        i++;
        if (i == 52)
            break;
        switch (i)
        {
        case 1:
            pid = atoi(info);
            break;
        case 3:
            state = info[0];
            break;
        case 8:
            tpgid = atoi(info);
            break;
        case 23:
            vmemory = atoll(info);
            break;
        default:
            continue;
        }
    }
    printf("pid -- %d\n", pid);
    if (tpgid == pid)
        printf("Process Status -- %c+\n", state);
    else
        printf("Process Status -- %c\n", state);

    printf("memory -- %llu {Virtual Memory}\n", vmemory);
    sprintf(buf, "/proc/%d/exe", pid);
    int execlen;
    if((execlen = readlink(buf, info, size))<0)
    {
        printf("Executable Path -- Could not get executable path\n");
        return;
    }
    info[execlen] = '\0';
    int n1 = strlen(home);
    int n2 = strlen(info);
    char path[MAX_SIZE];
    path[0] = '~';
    i = 1;
    int var = 0;
    if (strncmp(home, info, n1) == 0)
    {
        var = 1;
        while (info[n1 + i - 1] != '\0')
            path[i++] = info[n1 + i - 1];
        path[i] = '\0';
    }
    else
    {
        strcpy(path,info);
    }
    printf("Executable Path -- %s\n", path);
    fclose(procf);
    return;
}

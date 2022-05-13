#include "headers.h"

void prompt(){
    char *username;
    char systemname[MAX_SIZE];
    char curdir[MAX_SIZE];
    char dirname[MAX_SIZE];
    dirname[0] = '~';

    // get username & hostname
    username = getenv("USER");
    gethostname(systemname,MAX_SIZE);
    getcwd(curdir,MAX_SIZE);

    int n1 = strlen(home);
    int n2 = strlen(curdir);
    
    // get current directory relative to home directory
    if(strncmp(home,curdir,n1) == 0)
    {
        for(int i = n1;i < n2;i++)
        {
            dirname[i - n1 + 1] = curdir[i];
        }
        dirname[n2 - n1 + 1] = '\0';
    }
    else
    {
        strcpy(dirname,curdir);
    }
    
    // print prompt
    printf(BHGRN "<%s@%s:" CYN "%s>",username,systemname,dirname);
}

#include "headers.h"

void pwd()
{
    char cwd[MAX_SIZE];
    getcwd(cwd,MAX_SIZE);
    printf("%s\n",cwd);
    return;
}
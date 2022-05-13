#include <stdio.h>    
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include "colours.h"
#define MAX_SIZE 200

char home[MAX_SIZE];
char prevdir[MAX_SIZE];
char bgp[MAX_SIZE][MAX_SIZE];
int bgproc;
char lastcommand[MAX_SIZE];
char path[MAX_SIZE];
int foregroundprocpid;
struct foregroundproc
{
    int pid;
    char procname[MAX_SIZE];
};
struct foregroundproc *foreproc;
int shellpid;

int jobidx;
struct jobholder
{
    int jno;
    char state;
    int pid;
    char jobname[MAX_SIZE];
} jl;
struct jobholder *jh;

void    cd(char *command);
void    checkcommand(char *command);
void    redirec(char *command);
int     checkredir(char *command);
void    processcommands(char *commands);
int     checkpipe(char *command);
void    pipeit(char *command);
void    prompt();
void    echo(char *command);
void    pwd();
void    repeat(char *command);
int     ls(char *command);
int     lsl(char *fullpath, char *filename);
int     lsactual(char *dirname,int a_flag, int l_flag);
void    foreground(char *command);
void    background(char *command);
void    pinfo(char *command);
int     gethash(int n);
void    store(char *command,char *path);
void    gethistory(char *command,char *path);
void    jobs(char *command);
void    sendsignal(char *command);
void    fg(char *command);
void    bg(char *command);
void    redirecandpipe(char *command);
void    replay(char *command);
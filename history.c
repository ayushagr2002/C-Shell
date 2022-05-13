#include "headers.h"

void store(char *command, char *path)
{
    FILE* hist = fopen(path,"r");
    if(hist == NULL)
    {
        hist = fopen(path,"w");
    }
    if(lastcommand[0] == '\0')
        strcpy(lastcommand,command);
    else if(strcmp(lastcommand,command) == 0)
    {
        fclose(hist);
        return;
    }
    char buf1[21][MAX_SIZE];
    int j = 0;
    while(fgets(buf1[j],MAX_SIZE,hist))
    {
        j++;
    }
    if(j >= 20)
    {
        
        fclose(hist);
        hist = fopen(path,"w");
        int n1 = strlen(buf1[1]);
        fwrite(buf1[1],1,n1,hist);
        fclose(hist);
        hist = fopen(path,"a");
        for(int j = 2;j<20;j++)
        {
            int n2 = strlen(buf1[j]);
            fwrite(buf1[j],1,n2,hist);
        }
    }
    else
    {
        fclose(hist);
        hist = fopen(path,"a");
    }
        
    char buf2[MAX_SIZE];
    int n1 = strlen(command);
    strcpy(buf2,command);
    strcat(buf2,"\n");
    fwrite(buf2,1,n1+1,hist);
    strcpy(lastcommand,command);
    fclose(hist);
    return;
}

void gethistory(char *command,char *path)
{
    char buf[MAX_SIZE];
    strcpy(buf,command);
    char *saveptr;
    strtok_r(buf," ",&saveptr);
    int num;
    char *arg = strtok_r(NULL," ",&saveptr);
    if(arg == NULL)
        num = 10;
    else
        num = atoi(arg);
    
    if(num > 20)
    {
        printf("Please enter a number less than 20\n");
        return;
    }

    FILE* hist = fopen(path,"r");
    fseek(hist,0,SEEK_END);
    long size = ftell(hist);
    if(size >= 2010)
        size = 2010;

    char buf1[size+1];
    fseek(hist,-size,SEEK_END);
    fread(buf1,1,size,hist);
    buf1[size+1] = '\0';
    int count = 0;
    int pos = -1;
    for(int i = size + 1 ;i>=0;i--)
    {
        if(buf1[i] == '\n')
            count++;
        if(count == num + 1)
        {
            pos = i;
            break;
        }
    }
    buf1[size] ='\0';

    if(pos == -1)
        printf("%s",buf1);
    else
    {
        printf("%s",buf1+pos+1);
    }
    fclose(hist);
    return;
}
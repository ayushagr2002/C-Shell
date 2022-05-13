#include "headers.h"

int checkredir(char *command)
{
    int cmdlength = strlen(command);
    for(int i=0;i<cmdlength;i++)
    {
        if(command[i] == '<' || command[i] == '>')
            return 1;
    }
    return 0;
}
int checkpipe(char *command)
{
    int cmdlength = strlen(command);
    for(int i=0;i<cmdlength;i++)
    {
        if(command[i] == '|')
        {
            //printf("got pipe\n");
            return 1;
        }
    }
    return 0;
}
void redirec(char *command)
{
    
    int outputfile = 0, outputredirect = 0 ;
    int inputfile = 0, inputredirect = 0;
    int idxo = 0, idxi = 0;
    int append = 0;
    int pipe = 0;
    char outputfilename[MAX_SIZE], inputfilename[MAX_SIZE];
    int cmdlength = strlen(command);
    char buf[MAX_SIZE];
    strcpy(buf,command);
    char *saveptr;

    char *com = strtok_r(buf," ",&saveptr);
    com = strtok_r(NULL," ",&saveptr);
    while(com != NULL)
    {
        if(strcmp(com,"<") == 0)
        {
            inputredirect = 1;
            com = strtok_r(NULL," ",&saveptr);
            if(com == NULL)
            {
                printf("No input file provided\n");
                return;
            }
            strcpy(inputfilename,com);
        }
        if(com != NULL && strcmp(com,">") == 0)
        {
            outputredirect = 1;
            com = strtok_r(NULL," ",&saveptr);
            if(com == NULL)
            {
                printf("No output file provided\n");
                return;
            }
            strcpy(outputfilename,com);
        }
        if(com != NULL && strcmp(com,">>") == 0)
        {
            append = 1;
            outputredirect = 1;
            com = strtok_r(NULL," ",&saveptr);
            if(com == NULL)
            {
                printf("No output file provided\n");
                return;
            }
            strcpy(outputfilename,com);
            com = strtok_r(NULL," ",&saveptr);
        }
        com = strtok_r(NULL," ",&saveptr);
    }

    int pos = 0;
    for(int i=0;i<cmdlength;i++)
    {
        if(command[i] == '>' || command[i] == '<')
        {
            pos = i;
            break;
        }
    }

    char actual[MAX_SIZE];
    char actual2[MAX_SIZE];
    
    if(pos)
    {
        strncpy(actual,command,pos);
        actual[pos] = '\0';
    }
    else
        strcpy(actual,command);
    strcpy(actual2,actual);
    //printf("%s\n",actual);

    if(inputredirect && outputredirect)
    {
        inputfile = open(inputfilename,O_RDONLY);
        if(inputfile < 0)
        {
            perror("Could not open input file");
            return;
        }
        dup2(inputfile,STDIN_FILENO);
        if(append)
         outputfile = open(outputfilename, O_WRONLY|O_APPEND|O_CREAT, 0644);
        else
            outputfile = open(outputfilename,O_WRONLY|O_TRUNC|O_CREAT, 0644);
        if(outputfile < 0)
        {
            perror("Could not open output file");
            return;
        }
        dup2(outputfile,STDOUT_FILENO);
        close(inputfile);
        close(outputfile);
    }
    else if(inputredirect)
    {
        inputfile = open(inputfilename,O_RDONLY);
        if(inputfile < 0)
        {
            perror("Could not open input file");
            return;
        }
        dup2(inputfile,STDIN_FILENO);
        close(inputfile);
    }
    else if(outputredirect)
    {
        if(append)
         outputfile = open(outputfilename, O_WRONLY|O_APPEND|O_CREAT, 0644);
        else
            outputfile = open(outputfilename,O_WRONLY|O_TRUNC|O_CREAT, 0644);
        if(outputfile < 0)
        {
            perror("Could not open output file");
            return;
        }
        dup2(outputfile,STDOUT_FILENO);
        close(outputfile);
    }
    processcommands(actual2);
    return;
}

void pipeit(char *command)
{
    char buffer[MAX_SIZE];
    int cmdlength = strlen(command);
    int npipe = 0;
    int saved_stdout = dup(STDOUT_FILENO);
    int saved_stdin = dup(STDIN_FILENO);
    strcpy(buffer,command);
    char *saveptr;
    for(int i=0;i<cmdlength;i++)
    {
        if(command[i] == '|')
            npipe++;
    }
    //printf("%d\n",npipe);
    int pipearr[npipe][2];
    for(int i=0;i<npipe;i++)
    {
        if(pipe(pipearr[i]) < 0)
        {
            perror("Could not create pipe");
            return;
        }
    }
    char *arg = strtok_r(buffer,"|",&saveptr);
    
    dup2(pipearr[0][1],STDOUT_FILENO);
    close(pipearr[0][1]);

    if(checkredir(arg))
    {
        redirec(arg);
    }
    else
        processcommands(arg);
    
    //write(saved_stdout,"Completed writing to pipe\n",20);
    arg = strtok_r(NULL,"|",&saveptr);
    for(int i=1;i<npipe;i++)
    {
        dup2(pipearr[i-1][0],STDIN_FILENO);
        dup2(pipearr[i][1], STDOUT_FILENO);
        close(pipearr[i-1][0]);
        close(pipearr[i][1]);
        processcommands(arg);
        arg = strtok_r(NULL,"|",&saveptr);
    }
    dup2(pipearr[npipe-1][0],STDIN_FILENO);
    close(pipearr[npipe-1][0]);
    dup2(saved_stdout,STDOUT_FILENO);
    
    if(checkredir(arg))
        redirec(arg);
    else
        processcommands(arg);
    
    //write(saved_stdout,"Completed reading from pipe\n",10);
    dup2(saved_stdin,STDIN_FILENO);
   
    return;
}
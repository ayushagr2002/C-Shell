#include "headers.h"
long int total;

int getdir(char *command3)
{
    int n_dir = 0;
    char *saveptr2;
    char *temp = strtok_r(command3," ",&saveptr2);
    char *dirname = strtok_r(NULL," ",&saveptr2);
    while (dirname != NULL)
    {
        if(dirname[0] != '-')
            n_dir++;
        dirname = strtok_r(NULL," ",&saveptr2);
    }
    return n_dir;
}

int ls(char *command)
{
    char buf1[MAX_SIZE];
    char buf2[MAX_SIZE];
    strcpy(buf1, command);
    strcpy(buf2, command);
    char *saveptr1;
    char *temp = strtok_r(buf1, " ",&saveptr1);
    char *dirname = strtok_r(NULL, " ",&saveptr1);
    int a_flag = 0;
    int l_flag = 0;
    int return_val;

    int n_dir = getdir(buf2);
    if(n_dir == 0)
        n_dir = 1;
    //printf("%d\n",n_dir);

    // get the flags
    while (dirname != NULL)
    { 
        if(strcmp(dirname,"-l") == 0)
            l_flag = 1;
        else if(strcmp(dirname,"-a") == 0)
            a_flag = 1;
        else if(strcmp(dirname,"-al") == 0 || strcmp(dirname,"-la") == 0)
        {
            a_flag = 1;
            l_flag = 1;
        }
        dirname = strtok_r(NULL," ",&saveptr1);
    } 

    int shown = 0;
    char buf3[MAX_SIZE];
    strcpy(buf3,command);
    dirname = strtok_r(buf3," ",&saveptr1);
    dirname = strtok_r(NULL," ",&saveptr1);

    // for ls command with flags and directories
    while (dirname != NULL)
    {
        if (dirname[0] != '-')
        {
            if(n_dir > 1)
                printf("%s:\n",dirname);
            return_val = lsactual(dirname, a_flag, l_flag);
            if (return_val == -1)
                return -1;
            shown = 1;
        }
        dirname = strtok_r(NULL," ",&saveptr1);
    }

    // for ls command with only flags
    if (!shown)
    {
        return_val = lsactual(NULL, a_flag, l_flag);
        if (return_val == -1)
            return -1;
    }
    return 0;
}

int lsactual(char *dirname, int a_flag, int l_flag)
{
    DIR *folder;
    struct dirent *entry;
    int return_val;
    struct stat filetype;
    char filepath[MAX_SIZE];

    // set dirname appropriately
    if (dirname == NULL)
        dirname = ".";
    else if (strcmp(dirname, "~") == 0)
        dirname = home;

    // check if it is a file
    int exists = stat(dirname,&filetype);
    // if cant open the file throw error
    if(exists < 0)
    {
        char message[MAX_SIZE];
        strcpy(message, "ls cannot access ");
        strcat(message, dirname);
        perror(message);
        return -1;
    }
    // if file then do appropriate action
    if(S_ISREG(filetype.st_mode))
    {
        if(l_flag)
        {
            lsl(dirname,dirname);
        }
        else
            printf("%s\n",dirname);
        return 0;
    }

    // if not then open the dir
    folder = opendir(dirname);
    struct stat buf;
    long int total = 0;

    // if not able to open throw error and return
    if (folder == NULL)
    {
        char message[MAX_SIZE];
        strcpy(message, "ls cannot access ");
        strcat(message, dirname);
        perror(message);
        return -1;
    }

    // if l_flag is true, then calculate total
    if (l_flag)
    {
        while (entry = readdir(folder))
        {
            if (a_flag)
            {
                strcpy(filepath,dirname);
                strcat(filepath,"/");
                strcat(filepath,entry->d_name);
                if(stat(filepath, &buf) < 0)
                {
                    perror(filepath);
                    return -1;
                }
                total += buf.st_blocks;
            }
            else
            {
                if (entry->d_name[0] != '.')
                {
                    strcpy(filepath,dirname);
                    strcat(filepath,"/");
                    strcat(filepath,entry->d_name);
                    if(stat(filepath, &buf) < 0)
                    {
                        perror(filepath);
                        return -1;
                    }
                    total += buf.st_blocks;
                }
            }
        }
    }

    // if l_flag is true, print the total
    if(l_flag)
        printf("Total %ld\n",total/2);

    // move the pointer to begining of folder
    folder = opendir(dirname);
    while (entry = readdir(folder))
    {
        // for only a_flag
        if (a_flag && !l_flag)
        {
            if (entry->d_type == DT_DIR)
                printf("%s\n", entry->d_name);
            else
                printf("%s\n", entry->d_name);
        }

        else if (l_flag) // for l_flag
        {
            char filepath[MAX_SIZE];
            strcpy(filepath,dirname);
            strcat(filepath,"/");
            strcat(filepath,entry->d_name);   
            if (a_flag)
            {
                return_val = lsl(filepath,entry->d_name);
                if (return_val == -1)
                    return -1;
            }
            else
            {
                if (entry->d_name[0] != '.')
                {
                    return_val = lsl(filepath,entry->d_name);
                    if (return_val == -1)
                        return -1;
                }
            }
        }
        else // for without any flag
        {
            if (entry->d_name[0] != '.')
                    printf("%s\n", entry->d_name);
        }
    }
    closedir(folder);
    return 0;
}

int lsl(char *fullpath, char *filename)
{
    struct stat statbuf;
    struct passwd passbuf;
    struct passwd *passbufptr = &passbuf;
    if (stat(fullpath, &statbuf) < 0)
    {
        char message[MAX_SIZE];
        strcpy(message, "ls: Could not access ");
        strcat(message, filename);
        perror(message);
        return -1;
    }
    passbufptr = getpwuid(statbuf.st_uid);
    struct group *groupbuf = getgrgid(statbuf.st_gid);
    time_t tn = time(0);
    struct tm *timen = localtime(&tn);
    int tny = timen->tm_year + 1900;
    int tnm = timen->tm_mon + 1;
    int tnd = timen->tm_mday;
    struct tm *timebuf = localtime(&statbuf.st_mtim.tv_sec);
    int tfy = timebuf->tm_year + 1900;
    int tfm = timebuf->tm_mon + 1;
    int tfd = timebuf->tm_mday;
    char timeformat[30];
    
    if(tny - tfy >= 1)
    {
        strftime(timeformat,30,"%b %d  %Y",timebuf);
    }
    else if(tnm - tfm > 6)
        strftime(timeformat,30,"%b %d  %Y",timebuf);
    else if(tnm - tfm == 6)
    {
        if(tnd > tfd)
            strftime(timeformat,30,"%b %d  %Y",timebuf);
        else
            strftime(timeformat, 30, "%b %d %H:%M", timebuf);
    }
    else
        strftime(timeformat, 30, "%b %d %H:%M", timebuf);

    printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
    printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
    printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
    printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
    printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
    printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
    printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
    printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
    printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
    printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");
    printf(" %2d ", (int)statbuf.st_nlink);
    printf(" %s ", passbufptr->pw_name);
    printf(" %s ", groupbuf->gr_name);
    printf(" %5ld ", statbuf.st_size);
    printf(" %s ", timeformat);
    printf(" %s ", filename);
    printf("\n");
    return 0;
}
#include "headers.h"

void selsort(struct jobholder *jh, int jobidx)
{
    struct jobholder temp;

    for(int i=0;i<jobidx-1;i++)
    {
        for(int j=0;j<jobidx-i-1;j++)
        {
            if(strcmp(jh[j].jobname,jh[j+1].jobname)>0)
            {
                temp = jh[j];
                jh[j] = jh[j+1];
                jh[j+1] = temp;
            }
        }
    }
}

void jobs(char *command)
{
    char buf[MAX_SIZE];
    selsort(jh,jobidx);
    int cmdlen = strlen(command);
    int sflag=0, rflag=0;
    for(int i=0;i<cmdlen;i++)
    {
        if(command[i] == '-')
        {
            if(command[i+1] == 's')
                sflag = 1;
            if(command[i+1] == 'r')
                rflag = 1;
        }
    }
    for(int i=0;i<jobidx;i++)
    {   
        if(rflag || sflag)
        {
            if(rflag)
            {
                if(jh[i].state == 'R' && jh[i].jno != 0)
                    printf("[%d] Running %s [%d]\n",jh[i].jno,jh[i].jobname,jh[i].pid);
            }
            else
            {
                if(jh[i].state == 'T' && jh[i].jno != 0)
                    printf("[%d] Stopped %s [%d]\n",jh[i].jno,jh[i].jobname,jh[i].pid);
            }
        }
        else
        {
            if(jh[i].state == 'R' && jh[i].jno != 0)
            printf("[%d] Running %s [%d]\n",jh[i].jno,jh[i].jobname,jh[i].pid);
            else if(jh[i].jno != 0)
            printf("[%d] Stopped %s [%d]\n",jh[i].jno,jh[i].jobname,jh[i].pid);
        }
    }
    return;
}
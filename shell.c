#include "shell.h"


int main()
{
    // 
    char command[MAXLEN];
    char oldCmd[MAXLEN];
    int flag = 1, time = 0, status;
    while(flag)
    {
        printf("1612026_1512660> ");
        fflush(stdin);
        gets(command);
        if(strcmp(command, "exit") == 0 | strcmp(command, "q") == 0)
        {
            flag = 0;
            break;
        }
        else
        {
            time++;
            if(strcmp(command, "!!") == 0)
            {
                if(time == 0)
                {
                    printf("Err: No previous command line!!\n");
                }
                else
                {
                    strcpy(command, oldCmd);
                    if(strstr(command, "|") != NULL)
                    {
                        pipe_one(command);
                    }
                    else
                    {
                        if(strstr(command, ">") != NULL | strstr(command, "<") != NULL)
                        {
                            fileIO(command);
                        }
                        else
                        {
                            exec_nomal(command);
                        }
                    }
                }
            }
            else
            {
                strcpy(oldCmd, command);
                if(strstr(command, "|") != NULL)
                {
                    pipe_one(command);
                }
                else
                {
                    if(strstr(command, ">") != NULL | strstr(command, "<") != NULL)
                    {
                        fileIO(command);
                    }
                    else
                    {
                        exec_nomal(command);
                    }
                }
            }
        }
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#define MAXLEN 80
#define BUFLEN 256

pid_t spawn(const char * program, char * const * args)
{
    pid_t child_pid = fork();
    if (child_pid != 0)
    {
        return child_pid;
    }
    else
    {
        execvp(program, args);
        fprintf(stderr, "Error occurred in execvp.");
        int status;
        exit(status);
    }
}

void par_wait(pid_t child_pid, char* cmd)
{
    if(strstr(cmd, "&") == NULL)
    {
        waitpid(child_pid);
    }
}

int main()
{
    char command[MAXLEN];
    char oldCmd[MAXLEN];
    int flag = 1, time = 0;
    while(flag)
    {
        printf("1612026_1512660> ");
        fflush(stdin);
        gets(command);
        if(strcmp(command, "exit") == 0)
        {
            flag = 0;
            break;
        }
        else
        {
            if(strcmp(command, "!!") == 0)
            {
                if(time == 0)
                {
                    printf("Err: No previous command line!!\n");
                }
                else
                {
                    char * args[10];
                    int index = 0;
                    args[0] = strtok(oldCmd, " ");
                    while(args[index] != NULL)
                    {
                        index++;
                        args[index] = strtok(NULL, " ");
                    }
                    pid_t child_pid = spawn(args[0], args);
                    par_wait(child_pid, oldCmd);
                }
                
            }
            else
            {
                strcpy(oldCmd, command);
                if(strstr(command, "|") != NULL)
                {
                    int fd[2];
                    pipe(fd);
                }
                char * args[10];
                int index = 0;
                args[0] = strtok(command, " ");
                while(args[index] != NULL)
                {
                    index++;
                    args[index] = strtok(NULL, " ");
                }
                pid_t child_pid = spawn(args[0], args);
                waitpid(child_pid);
                time++;
            }
        }
    }
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#define MAXLEN 80
#define BUFLEN 256
#define WRITE_END 1
#define READ_END 0

char *currentDirectory;

void fileIO(char *command)
{
    int option;
    char *cmd, *file, *args[10];
    int id = 0;
    if(strstr(command, ">") != NULL)
    {
        option = 0;
        cmd = strtok(command, ">");
        file = strtok(NULL, ">");
        
    }
    else
    {
        option = 1;
        cmd = strtok(command, "<");
        file = strtok(NULL, "<");
    }

    args[0] = strtok(cmd, " ");
    while(args[id] != NULL)
    {
        id++;
        args[id] = strtok(NULL, " ");
    }

    while(file[0] == ' ')
	{
        int n = strlen(file);
        for(int i = 0; i < n - 1; i++)
     		file[i] = file[i + 1];
        file[n - 1]='\0';
    }

    pid_t child_pid = fork();
    if(child_pid == 0)
    {
        if(option == 0)
        {
            out(file, args);
        }
        else
        {
            in(file, args);
        }
        fprintf(stderr, "Error occurred in execvp.\n");
        int status;
        exit(status);
    }
    waitpid(child_pid, NULL, 0);
}

void out(char *file, char *args[])
{
    int out;
    // open input and output files
    out = open(file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    // replace standard output with output file
    dup2(out, 1);
    // close unused file descriptors
    close(out);
    execvp(args[0], args);
}

void in(char *file, char *args[])
{
    int in;
    in = open(file, O_RDONLY);
    // replace standard input with input file
    dup2(in, 0);
    // close unused file descriptors
    close(in);
    execvp(args[0], args);
}

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
    // don't wait if cmd has &
    if(strstr(cmd, "&") == NULL)
    {
        waitpid(child_pid, NULL, 0);
    }
}

void pipe_one(char *command)
{
    char *firstcmd, *scmd;
    firstcmd = strtok(command, "|");
    scmd = strtok(NULL, "|");

    char *frsarg[10], *secarg[10];
    int id = 0;
    frsarg[0] = strtok(firstcmd, " ");
    while(frsarg[id] != NULL)
    {
        id++;
        frsarg[id] = strtok(NULL, " ");
    }
    id = 0;
    secarg[0] = strtok(scmd, " ");
    while(secarg[id] != NULL)
    {
        id++;
        secarg[id] = strtok(NULL, " ");
    }

    pid_t pid;
    int fd[2];
    pipe(fd);
    pid = fork();
    if(pid == 0)
    {
        dup2(fd[WRITE_END], STDOUT_FILENO);
        close(fd[READ_END]);
        close(fd[WRITE_END]);
        execlp(frsarg[0], frsarg[0], frsarg[1], (char*) NULL);
        fprintf(stderr, "Failed to execute '%s'\n", firstcmd);
        exit(1);
    }
    else
    { 
        pid = fork();
        if(pid == 0)
        {
            dup2(fd[READ_END], STDIN_FILENO);
            close(fd[WRITE_END]);
            close(fd[READ_END]);
            execlp(secarg[0], secarg[0], secarg[1],(char*) NULL);
            fprintf(stderr, "Failed to execute '%s'\n", scmd);
            exit(1);
        }
        else
        {
            close(fd[READ_END]);
            close(fd[WRITE_END]);
            waitpid(pid, NULL, 0);
        }
    }
}

void exec_nomal(char *command)
{
    char * args[10];
    int index = 0;
    args[0] = strtok(command, " ");
    while(args[index] != NULL)
    {
        index++;
        args[index] = strtok(NULL, " ");
    }
    pid_t child_pid = spawn(args[0], args);
    par_wait(child_pid, command);
}

void FrsSpaceErase(char *s)
{
	int n=strlen(s);
    if(s[0] == ' ')
		for(int i = 0; i < n - 1; i++)
     		s[i] = s[i + 1];
	s[n - 1]='\0';
}
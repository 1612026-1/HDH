#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "grep Villanova < scores > out".
 *
 * @author Jim Glenn
 * @version 0.1 9/23/2004
 */

int main(int argc, char **argv)
{
  char *args[] = {"wc", NULL};
  char *file = "inout.txt";
  in(file, args);
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
//
/// Gissel Santoyo
/// CS 3100
/// Challenge 2: Write a shell
/// Professor Zhang
/// Due: 5/25/23
/// File: shell.C
/// This is a short program containing 4 parts:
/// 1. Prints a welcome message + working directory that loops
/// 2. Takes user input, parses their command
///    and determines if the command is internal or external
/// 3. Has internal (built in) commands for cd and quit
/// 4. Uses a fork to execute external commands
//


#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <sys/wait.h>

// a method that takes in an input and parses it into an array
void parse(char *input, char **command)
{
     // while input is not over
     while (*input != '\0')
     {
          // while input is a space, tab or newline, replace with a \0
          while (*input == ' ' || *input == '\t' || *input == '\n')
          {
               *input++ = '\0';
          }

          // save input into command(array of pointers)
          *command++ = input;

          // while input is not a space, tab, newline and a \0
          while (*input != ' ' && *input != '\t' && *input != '\n' && *input != '\0')
          {
               // go to next part of input
               input++;
          }
     }

     // set last entry of arg to a NULL pointer
     *command = NULL;
}

// A method that performs a change in directory
void cd(char **command, char *path)
{
     // if the command entered has a directory following cd
     if (command[1] != NULL)
     {
          // change to directory input
          chdir(command[1]);
          getcwd(path, 100);
     }
     else
     {
          // else if there is nothing following cd
          // go back a directory
          chdir("..");
          getcwd(path, 100);
     }

}

int main(void)
{
     // create a variable to store the path and user input
     char path[100];
     char input[1024];

     // create a variable to store pointers for when input is parsed
     char *command[64];

     // print shell welcome and get current path
     printf("Welcome to my shell{} !!\n");
     if (getcwd(path, sizeof(path)) != NULL)
     {
          printf("Current directory: %s\n", path);
     }
     else
     {
          perror("getcwd() error");
          return 1;
     }

     while (1)
     {
          // print shell prompt, take in input, then pass to parse function
          printf("\n%s>", path);
          scanf("%[^\n]%*c", input);
          parse(input, command);

          // if input is an internal command, manually perform command
          if (strcmp(command[0], "quit") == 0)
          {
               // exit while loop if quit is entered
               exit(0);
          }
          if (strcmp(command[0], "cd") == 0)
          {
               // execute cd command function if cd is entered
               cd(command, path);
          }
          else
          {
               // using fork for external commands
               pid_t pid;
               int status;
               pid = fork();

               // fork failed
               if (pid < 0)
               {
                    printf("*** ERROR: forking child process failed\n");
                    exit(1);
               }
               // Child process
               else if (pid == 0)
               {
                    // execution of external command 
                    if (execvp(*command, command) < 0)
                    {
                         // if execution fails, print error
                         printf("*** ERROR: exec failed\n");
                         exit(1);
                    }
               }
               else
               { 
                    // parent waits
                    pid = wait(NULL);
               }
          }
     }
     return 0;
}
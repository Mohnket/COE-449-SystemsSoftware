////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// File Name: myshell.c
///
/// @par Full Description
/// COE 449 Project 4
/// A half assed shell
///
/// @version
/// 09-Apr-2018 Tyler Mohnke    Initial implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// System includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/times.h>

// Constants
#define INPUT_SIZE 1000
#define NUMBER_OF_TOKES 100
#define DELIMINATORS " \t\n()<>|&;"

// File Scoped Variables
static char f_pInput[INPUT_SIZE];
static char f_pInputCopy[INPUT_SIZE];
static char* f_ppTokens[NUMBER_OF_TOKES];
static int f_ArgIndex = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: cmd
///
/// @par Full Description
/// runs the current cmd
///
/// @param index    The starting index of the command
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cmd(int index)
{
    char* append = strstr(f_pInputCopy, ">>");
    char* write = strstr(f_pInputCopy, ">");
    
    if(append != NULL)
    {
        f_ArgIndex--;
        FILE* redirect = freopen(f_ppTokens[f_ArgIndex], "a", stdout);
        if(redirect == NULL)
        {
            printf("Unable to open file: %s", f_ppTokens[f_ArgIndex]);
            exit(1);
        }
        f_ppTokens[f_ArgIndex] = NULL;
    }
    else if(write != NULL)
    {
        f_ArgIndex--;
        FILE* redirect = freopen(f_ppTokens[f_ArgIndex], "w", stdout);
        if(redirect == NULL)
        {
            printf("Unable to open file: %s", f_ppTokens[f_ArgIndex]);
            exit(1);
        }
        f_ppTokens[f_ArgIndex] = NULL;
    }
    else
    {
       
    }
    
    int success = execvp(f_ppTokens[index], &f_ppTokens[index]);
    if(success == -1)
    {
        printf("Invalid command");
        exit(1);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: cd
///
/// @par Full Description
/// Changes the current dircectory
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cd()
{
    int success = chdir(f_ppTokens[1]);
    if(success == -1)
    {
        printf("Directory not found\n");
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: shell
///
/// @par Full Description
/// Runs a shell command
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void shell()
{    
    if(strcmp(f_ppTokens[0], "exit") == 0)
    {
        exit(0);
    }
    else if(strcmp(f_ppTokens[0], "cd") == 0)
    {
        cd();
    }
    else if(strcmp(f_ppTokens[0], "time") == 0)
    {
        if(fork() == 0)
        {
            cmd(1);
            exit(0);
        }
        else
        {
            struct tms before;
            struct tms after;
            
            times(&before);
            
            int status;
            wait(&status);
            
            times(&after);
            
            printf("User time: %d\n", after.tms_cutime - before.tms_cutime);
            printf("System time: %d\n", after.tms_cstime - before.tms_cstime);
        }
    }
    else
    {
        if(fork() == 0)
        {
            cmd(0);
            exit(0);
        }
        else
        {
            int status;
            wait(&status);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: main
///
/// @par Full Description
/// Entry point of the program
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    for(;;)
    {
        printf("\n>>> ");
        fgets(f_pInput, INPUT_SIZE, stdin);
        strcpy(f_pInputCopy, f_pInput);
        
        f_ppTokens[f_ArgIndex] = strtok(f_pInput, DELIMINATORS);
        while(f_ppTokens[f_ArgIndex] != NULL)
        {
            f_ArgIndex++;
            f_ppTokens[f_ArgIndex] = strtok(NULL, DELIMINATORS);
        }
        
        shell();
        
        f_ArgIndex = 0;
    }
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

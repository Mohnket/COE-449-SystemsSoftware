////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// File Name: rsp.c
///
/// @par Full Description
/// COE 449 Project 1
/// Rock, Paper, Scissors implementation
///
/// @version
/// 24-Jan-2018 Tyler Mohnke    Initial implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// System Includes
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Constant Values
#define INPUT_SIZE 30
#define LOWER_CASE_OFFSET 0x20
#define GAMES_TO_WIN 3

#define ROCK 0
#define PAPER 1
#define SCISSORS 2
#define RAND_MODULO 3

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: MakeLower
///
/// @par Full Description
/// Replaces all uppercase letters in the given string with lower case letters
///
/// @param str  [in/out] The input string
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MakeLower(char* str)
{
    while(*str != '\0')
    {
        if(('A' <= *str) && (*str <= 'Z'))
        {
            *str = *str + LOWER_CASE_OFFSET;
        }
        str++;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: main
///
/// @par Full Description
/// Entry point of the program
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    printf("Welcome to Rock, Paper, Scissors\n\n");
    printf("Would you like to play? ");
    
    char aUserInput[INPUT_SIZE];
    scanf("%s", aUserInput);
    MakeLower(aUserInput);
    
    while((strcmp(aUserInput, "y") == 0) || (strcmp(aUserInput, "yes") == 0))
    {
        uint8_t playerScore = 0U;
        uint8_t computerScore = 0U;
        
        srand((unsigned int)time(NULL));
        
        while((playerScore < GAMES_TO_WIN) && (computerScore < GAMES_TO_WIN))
        {
            printf("\nWhat is your choice? ");
            scanf("%s", aUserInput);
            MakeLower(aUserInput);
            
            int computerSelection = rand( )% RAND_MODULO;
            switch(computerSelection)
            {
                case ROCK:
                {
                    printf("The computer chooses rock. ");
                    if((strcmp(aUserInput, "p") == 0) || (strcmp(aUserInput, "paper") == 0))
                    {
                        printf("You win this game!\n\n");
                        playerScore++;
                    }
                    else if((strcmp(aUserInput, "s") == 0) || (strcmp(aUserInput, "scissors") == 0))
                    {
                        printf("You lose this game!\n\n");
                        computerScore++;
                    }
                    else
                    {
                        printf("Tie game\n\n");
                    }
                }
                break;
                
                case PAPER:
                {
                    printf("The computer chooses paper. ");
                    if((strcmp(aUserInput, "s") == 0) || (strcmp(aUserInput, "scissors") == 0))
                    {
                        printf("You win this game!\n\n");
                        playerScore++;
                    }
                    else if((strcmp(aUserInput, "r") == 0) || (strcmp(aUserInput, "rock") == 0))
                    {
                        printf("You lose this game!\n\n");
                        computerScore++;
                    }
                    else
                    {
                        printf("Tie game\n\n");
                    }
                }
                break;
                
                case SCISSORS:
                {
                    printf("The computer chooses scissors. ");
                    if((strcmp(aUserInput, "r") == 0) || (strcmp(aUserInput, "rock") == 0))
                    {
                        printf("You win this game!\n\n");
                        playerScore++;
                    }
                    else if((strcmp(aUserInput, "p") == 0) || (strcmp(aUserInput, "paper") == 0))
                    {
                        printf("You lose this game!\n\n");
                        computerScore++;
                    }
                    else
                    {
                        printf("Tie game\n\n");
                    }
                }
                break;
            }
            
            printf("The score is now you: %d computer: %d\n\n", playerScore, computerScore);
        }
        
        if(playerScore == GAMES_TO_WIN)
        {
            printf("You win. Would you like to play again? ");
        }
        else
        {
            printf("You lose. Would you like to play again? ");
        }
        
        scanf("%s", aUserInput);    
        MakeLower(aUserInput);
    }
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// File Name: mystrings.c
///
/// @par Full Description
/// COE 449 Project 2
/// Finds all c-style text strings in a given file
/// Usage: mystrings file_name
///
/// @version
/// 01-Mar-2018 Tyler Mohnke    Initial implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// System Includes
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Constant values
#define ARGUMENT_COUNT 2
#define FILE_NAME_OFFSET 1
#define BUFFER_SIZE 100
#define MINIMUM_STRING_LENGTH 4
#define PRINTABLE_CHAR_LOWER_BOUND (char)32
#define PRINTABLE_CHAR_UPPER_BOUND (char)126

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: IsPrintableChar
///
/// @par Full Description
/// Determines whether the given character is printable or not
///
/// @param character    The char to be evaluated
///
/// @return true    if character is printable
///         false   otherwise
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t IsPrintableChar(char character)
{
    return ((PRINTABLE_CHAR_LOWER_BOUND <= character) && (character <= PRINTABLE_CHAR_UPPER_BOUND));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: CheckBufferSize
///
/// @par Full Description
/// Determines whether the dynamic buffer is about to overflow, and expans it if so
///
/// @param pBuffer  A dynamicaly allocated buffer
/// @param pSize    A reference to the size of the buffer
/// @param index    The index about to be accessed of the buffer
///
/// @return char*   If there is room for another char pBuffer is returned, otherwise a new buffer twice the size of 
///                 pSize is returned
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char* CheckBufferSize(char* pBuffer, uint32_t* pSize, uint32_t index)
{
    if(index == *pSize)
    {
        char* pNewBuffer = (char*)malloc((*pSize) * 2);
        if(pNewBuffer == NULL)
        {
            printf("Error: Memory not allocated");
            exit(EXIT_FAILURE);
        }
        
        memcpy(pNewBuffer, pBuffer, *pSize);
        *pSize *= 2;
        free(pBuffer);
        pBuffer = pNewBuffer;
    }
    
    return pBuffer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: main
///
/// @par Full Description
/// Entry point of the program
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    // Check arguments
    if(argc != ARGUMENT_COUNT)
    {
        printf("Usage: mystrings file_name\n");
        exit(EXIT_FAILURE);
    }
    
    // Check for file existance
    FILE* pInputFile = fopen(argv[FILE_NAME_OFFSET], "rb");
    if(pInputFile == NULL)
    {
        printf("Error: %s not found\n", argv[FILE_NAME_OFFSET]);
        exit(EXIT_FAILURE);
    }
    
    // Allocate an outputbuffer
    uint32_t outputBufferSize = BUFFER_SIZE;
    char* pOutputBuffer = (char*)malloc(outputBufferSize);
    if(pOutputBuffer == NULL)
    {
        printf("Error: Memory not allocated");
        exit(EXIT_FAILURE);
    }
    uint32_t outputIndex = 0U;
    
    // Read in the input file into a local buffer
    char aInputBuffer[BUFFER_SIZE];
    uint32_t bytesRead = fread(&aInputBuffer[0U], BUFFER_SIZE * sizeof(char), 1, pInputFile);
    
    // While the entire file has not been read, try to find any strings in the buffer
    while(bytesRead != 0)
    {
        // for each character in the input buffer if it is printable, place it into the output buffer, else attempt to
        // print out the output buffer
        uint32_t inputIndex;
        for(inputIndex = 0U; inputIndex < BUFFER_SIZE; ++inputIndex)
        {
            // resize the output buffer if necessary
            pOutputBuffer = CheckBufferSize(pOutputBuffer, &outputBufferSize, outputIndex);
            
            if(IsPrintableChar(aInputBuffer[inputIndex]))
            {
                pOutputBuffer[outputIndex] = aInputBuffer[inputIndex];
                outputIndex++;
            }
            else
            {
                // if the output length is grater than the minimum print it to the console
                if(outputIndex >= MINIMUM_STRING_LENGTH)
                {
                    pOutputBuffer[outputIndex] = '\0';
                    printf("%s\n", pOutputBuffer);
                }
                
                outputIndex = 0;
            }
        }
        
        bytesRead = fread(&aInputBuffer[0U], BUFFER_SIZE * sizeof(char), 1, pInputFile);
    }
    
    free(pOutputBuffer);
    
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

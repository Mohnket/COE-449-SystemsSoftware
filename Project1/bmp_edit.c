////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// File Name: bmp_edit.c
///
/// @par Full Description
/// COE 449 Project 1
/// Command line image editor
/// Usage: bmp_edit [-invert | -grayscale] file_name.bmp
///
/// @version
/// 08-Feb-2018 Tyler Mohnke    Initial implementation
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// System Includes
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Constant Values
#define ARGUMENT_COUNT 3
#define OPTION_OFFSET 1
#define FILE_NAME_OFFSET 2
#define FORMAT_IDENTIFIER (*((uint16_t*)"BM"))
#define BITS_PER_PIXEL 24
#define BYTES_PER_PIXEL 3
#define MULTIPLE_OF_BMP_ROW 4

#define BLUE_OFFSET 0
#define GREEN_OFFSET 1
#define RED_OFFSET 2

#define BLUE_MULTIPLIER .0722
#define GREEN_MULTIPLIER .7152
#define RED_MULTIPLIER .2126

#define LUMINANCE_LINEAR_BOUND .0031308
#define LUMINANCE_LINEAR_MULTIPLIER 12.92
#define LUMINANCE_EXPONENT_MULTIPLIER 1.055
#define LUMINANCE_EXPONENT 1/2.4
#define LUMINANCE_EXPONENT_OFFSET .055

#pragma pack(push, 1)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Struct Name: BmpHeader
///
/// @par Full Description
/// Header format of of a .bmp file
/// 
/// @note The stuct is packed
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    uint16_t m_FormatIdentifier;
    uint32_t m_FileSize;
    uint16_t RESERVED1;
    uint16_t RESERVED2;
    uint32_t m_PixelArrayOffset;
} BmpHeader;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Struct Name: DibHeader
///
/// @par Full Description
/// format of a DIB data structure
///
/// @note The stuct is packed
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    uint32_t m_HeaderSize;
    uint32_t m_Width;
    uint32_t m_Height;
    uint16_t m_ColorPlanes;
    uint16_t m_BitsPerPixel;
    uint32_t m_CompressionScheme;
    uint32_t m_ImageSize;
    uint32_t m_HorizontalResolution;
    uint32_t m_VerticalResolution;
    uint32_t m_NumberOfColors;
    uint32_t m_NumberOfImportantColors;
} DibHeader;
#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: Round
///
/// @par Full Description
/// Rounds the value to the nearest multiple of a different value
///
/// @param integer  The value to round
/// @param multiple The multiple value to round towards
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t Round(uint32_t integer, uint32_t multiple)
{
    if(multiple == 0U)
    {
        return integer;
    }
    
    uint32_t remainder = integer % multiple;
    if(remainder == 0U)
    {
        return integer;
    }
    
    return remainder = integer + multiple - remainder;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: Invert
///
/// @par Full Description
/// Replaces all values in the given array with thier bitwise not value
///
/// @param pArr An array of uint8_t
/// @param size The size of the array
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Invert(uint8_t* pArr, uint32_t size)
{
    for(uint32_t index = 0U; index < size; ++index)
    {
        pArr[index] = (uint8_t)(~pArr[index]);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function Name: MakeGrayScale
///
/// @par Full Description
/// Replaces all pixels in the array with their grayed out values
///
/// @param pArr An array of uint8_t
/// @param size The size of the array
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MakeGrayScale(uint8_t* pArr, uint32_t size)
{
    for(uint32_t index = 0U; index < size; index += BYTES_PER_PIXEL)
    {
        double red = (double)pArr[index + RED_OFFSET] / (double)UINT8_MAX;
        double blue = (double)pArr[index + BLUE_OFFSET] / (double)UINT8_MAX;
        double green = (double)pArr[index + GREEN_OFFSET] / (double)UINT8_MAX;
        
        double luminance = (RED_MULTIPLIER * red) + (BLUE_MULTIPLIER * blue) + (GREEN_MULTIPLIER * green);
        
        double grayValue; 
        if(luminance <= LUMINANCE_LINEAR_BOUND)
        {
            grayValue = luminance * LUMINANCE_LINEAR_MULTIPLIER;
        }
        else
        {
            grayValue = LUMINANCE_EXPONENT_MULTIPLIER * pow(luminance, LUMINANCE_EXPONENT) - LUMINANCE_EXPONENT_OFFSET;
        }
        
        grayValue *= (double)UINT8_MAX;
        pArr[index + RED_OFFSET] = (uint8_t)grayValue;
        pArr[index + BLUE_OFFSET] = (uint8_t)grayValue;
        pArr[index + GREEN_OFFSET]  = (uint8_t)grayValue;
    }
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
    // check struct size
    assert(sizeof(BmpHeader) == 14);
    assert(sizeof(DibHeader) == 40);
    
    // Check arguments
    if((argc != ARGUMENT_COUNT) ||
       ((strcmp(argv[OPTION_OFFSET], "-invert") != 0) && (strcmp(argv[OPTION_OFFSET], "-grayscale") != 0)))
    {
        printf("Usage: bmp_edit [-invert | -grayscale] file_name.bmp\n");
        return 0;
    }
    
    // Check for file existance
    FILE* pBmpFile = fopen(argv[FILE_NAME_OFFSET], "r+b");
    if(pBmpFile == NULL)
    {
        printf("%s not found\n", argv[FILE_NAME_OFFSET]);
        return 0;
    }
    
    // Read the first header
    BmpHeader bmpHeader;
    fread(&bmpHeader, sizeof(uint8_t), sizeof(BmpHeader), pBmpFile);
    if(bmpHeader.m_FormatIdentifier != FORMAT_IDENTIFIER)
    {
        printf("File format not supported.\n");
        return 0;
    }
    
    // Read the second header
    DibHeader dibHeader;
    fread(&dibHeader, sizeof(uint8_t), sizeof(DibHeader), pBmpFile);
    if(dibHeader.m_HeaderSize != sizeof(DibHeader))
    {
        printf("File format not supported.\n");
        return 0;
    }
    else if(dibHeader.m_BitsPerPixel != BITS_PER_PIXEL)
    {
        printf("File format not supported.\n");
        return 0;
    }
    else
    {
        
    }
    
    // prepare to esit image
    fseek(pBmpFile, bmpHeader.m_PixelArrayOffset, SEEK_SET);
    
    const uint32_t rowSize = dibHeader.m_Width * BYTES_PER_PIXEL;
    const uint32_t paddedRowSize = Round(rowSize, MULTIPLE_OF_BMP_ROW);
    
    uint8_t* pPixelRow = (uint8_t*)malloc(rowSize);
    if(pPixelRow != NULL)
    {
        // For every row; read in the row. Perform the transformation. Write the new row
        for(uint32_t rowIndex = 0U; rowIndex < dibHeader.m_Height; ++rowIndex)
        {
            // read row
            fseek(pBmpFile, bmpHeader.m_PixelArrayOffset + (rowIndex * paddedRowSize), SEEK_SET);
            fread(pPixelRow, sizeof(uint8_t), rowSize, pBmpFile);

            // transform row
            // argument check gurrenteed invert or grayscale; only need to check one character
            if(argv[OPTION_OFFSET][1U] == 'i')
            {
                Invert(pPixelRow, rowSize);
            }
            else
            {
                MakeGrayScale(pPixelRow, rowSize);
            }
                
            // write row
            fseek(pBmpFile, bmpHeader.m_PixelArrayOffset + (rowIndex * paddedRowSize), SEEK_SET);
            fwrite(pPixelRow, sizeof(uint8_t), rowSize, pBmpFile);
        }
    }
    else
    {
        printf("Out of memory, image not converted\n");
    }
    
    free(pPixelRow);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * resize.c
 *
 */
       
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#include "bmp.h"


int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy N infile outfile\n");
        return 1;
    }


    //  get the resize size
    double ResizeFactorFloat = atof(argv[1]);
    int ResizeFactorInt;
    if (ResizeFactorFloat < 0) {
        printf("Usage: put a positive argument in please \n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
    
    
    // Set them in the short term:
    ResizeFactorInt = (int) ResizeFactorFloat;
//    printf("  Resize Factor Float %2f\n", ResizeFactorFloat);
//    printf("  Resize Factor Int %i\n", ResizeFactorInt);


    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w+");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Copy bitmap Details to Bitmap Out    
    BITMAPFILEHEADER bfOut;
    BITMAPINFOHEADER biOut;
    memcpy(&bfOut, &bf, sizeof(BITMAPFILEHEADER));
    memcpy(&biOut, &bi, sizeof(BITMAPINFOHEADER));
    
    // Determine the new Size details
    biOut.biWidth = bi.biWidth * ResizeFactorInt;
    biOut.biHeight = bi.biHeight * ResizeFactorInt;
    
    // determine padding for scanlines -  this needs to be calculated before in order to determine new variables
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddingOut =  (4 - (biOut.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Adjust the other Header Values to be written
    biOut.biSizeImage = ((biOut.biWidth * sizeof(RGBTRIPLE)) + paddingOut) * abs(biOut.biHeight);
    bfOut.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + biOut.biSizeImage;
    

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfOut, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biOut, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // Prepare the variable for increasing size
    int RowLength = bi.biWidth * sizeof(RGBTRIPLE) * ResizeFactorInt;
    
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {

        // temporary storage
        RGBTRIPLE triple;

       // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
 
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            //  This is where we have to multiply out the usage of rows and columns using fseek
            //
            // Finish off writing the whole row for this value of i
            for (int ExtraCols = 0; ExtraCols < ResizeFactorInt; ExtraCols++ )
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }
        
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add new padding back
        for (int k = 0; k < paddingOut; k++)
        {
            fputc(0x00, outptr);
        }

        // now write the extra rows - remember one has already been written
        for (int ExtraRows = 0; ExtraRows < ResizeFactorInt - 1; ExtraRows++)
        {
            // then skip back in front of any padding
            for (int ExtraCols = 0; ExtraCols < (ResizeFactorInt * bi.biWidth); ExtraCols++)
            {
                // move the pointer back to last identical triple
                fseek(outptr, - RowLength - paddingOut, SEEK_CUR);
                fread(&triple, sizeof(RGBTRIPLE), 1, outptr);
                fseek(outptr, RowLength - sizeof(RGBTRIPLE) + paddingOut, SEEK_CUR);
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            // then add padding to each line
            for (int k = 0; k < paddingOut; k++)
            {
                fputc(0x00, outptr);
            }

        }


    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}

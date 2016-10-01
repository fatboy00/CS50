/**
 * resize.c
 * Hacker version
 *
 */
       
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<math.h>
#include<cs50.h>


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
    if (ResizeFactorFloat > 100) {
        printf("Usage: put an argument less than 100 in please \n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
    
    
    // Set them in the short term:
    ResizeFactorInt = (int) ResizeFactorFloat;


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
    biOut.biWidth = round(((double) bi.biWidth) * ResizeFactorFloat);
    biOut.biHeight = round(((double) bi.biHeight) * ResizeFactorFloat);

    // determine padding for scanlines -  this needs to be calculated before in order to determine new variables
//    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int paddingOut =  (4 - (biOut.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Adjust the other Header Values to be written
    biOut.biSizeImage = ((biOut.biWidth * sizeof(RGBTRIPLE)) + paddingOut) * abs(biOut.biHeight);
    bfOut.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) + biOut.biSizeImage;
    

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfOut, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biOut, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // Prepare the variable for increasing size

    // calculate Weights for each Row & Column
    // First need to calculate Which Source Pixels apply to which Target Pixels
    // Set up Variables
    double TargetWidthDist[biOut.biWidth];
    double TargetHeightDist[abs(biOut.biHeight)];
    double SourceWidthDist[bi.biWidth];
    double SourceHeightDist[abs(bi.biHeight)];
    double WeightWidth[biOut.biWidth];  
    double WeightHeight[abs(biOut.biHeight)];  
    int SourceLocationWidth[biOut.biWidth];
    int SourceLocationHeight[abs(biOut.biHeight)];

    // do Row (to match Scanline) first
    // do source first - so loop only repeated once
    for (int j = 0; j < bi.biWidth; j++) 
    {

        SourceWidthDist[j] = 0.5 + (double) j;

        //  now save time because this program will only do square increases
        SourceHeightDist[j] = SourceWidthDist[j];
    }

    // now target
    for (int i = 0; i<biOut.biWidth; i++) 
    {
        int j = 0;
        bool found = false;

        TargetWidthDist[i] = (((double) i * (double) bi.biWidth) + ( (double) bi.biWidth / 2)) / (double) biOut.biWidth;

        //  now save time because this program will only do square increases
        TargetHeightDist[i] = TargetWidthDist[i];

        while (!found) 
        {
            if (SourceWidthDist[j] < TargetWidthDist[i]) 
            {
                j++;
            }
            else 
            {
                found = true;
            }
        }
        j--;

        if (j < 0)
        {
            SourceLocationWidth[i] = 0;
        } else
        {
            SourceLocationWidth[i] = j;
        }
        //  now save time because this program will only do square increases
        SourceLocationHeight[i] = SourceLocationWidth[i];

        WeightWidth[i] =  TargetWidthDist[i] -  SourceWidthDist[j];
        //  need to deal with start and end exceptions
        if (j < 0)
        {
            WeightWidth[i] = 0;
        }
        else if (j == bi.biWidth) 
        {
            WeightWidth[i] = 1;
        }
        //  now save time because this program will only do square increases
        WeightHeight[i] = WeightWidth[i];


    }   

        
    // Create an input and output memory space that stores the entire .BMP
    
    unsigned char *inpointer = malloc(bi.biSizeImage);
    unsigned char *outpointer = malloc(biOut.biSizeImage);


    // get the input BMP
    fread(inpointer, bi.biSizeImage, 1, inptr);

    // Do Rows or Source BMP
    for (int i = 0; i < abs(biOut.biHeight); i++) // Target does not have same number of rows....
    {
        // Loop Through Target Columns
        // Set Counters
        
        for (int j = 0; j < biOut.biWidth; j++)
        {
        //  Ok.  Lets find the source Pixels!!
            int s = sizeof(RGBTRIPLE);
            int row = (i * (biOut.biWidth * s + paddingOut));
            int Index = (j * s) + row;
            int k =  SourceLocationWidth[j];       
            int l =  SourceLocationHeight[i];       
            int rowS = (l * (bi.biWidth * s + paddingOut));
            int rowS1 = ((l + 1) * (bi.biWidth * s + paddingOut));
            double w = WeightWidth[j];
            double wr = WeightHeight[i];
            

            for (int c = 0; c < s; c++) // Iterate through the colours
            {
                BYTE RowUp;
                BYTE RowDown;
                RowUp = inpointer[k * s + rowS + c] + w * (inpointer[(k + 1) * s + rowS + c] - inpointer[k * s + rowS + c]);
                RowDown = inpointer[k * s + rowS1 + c] + w * (inpointer[(k + 1) * s + rowS1 + c] - inpointer[k * s + rowS1 + c]);
                outpointer[Index + c] = RowUp + wr * (RowDown - RowUp);
            }
        }  // end loop on columns
    } // end of for loop on rows

    // write the BMP
    fwrite(outpointer, biOut.biSizeImage, 1, outptr);

    // free space
    free (inpointer);
    free (outpointer);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}

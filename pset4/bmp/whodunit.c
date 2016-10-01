/**
 * whodunit.c
 *
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"

// prototypes
void ChangePixel();

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[1];
    char* outfile = argv[2];
    
    
    //  create three different variations of stripping Pixels to save time

    char body[100];
    int len = strlen (outfile);
    for (int i = 0; i < len -4; i++ ) {
        body[i] = outfile[i];
    }

    char outfile1[100];
    strcpy(outfile1, body);
    strcat(outfile1, "1");
    strcat(outfile1, ".bmp");

    char outfile2[100];
    strcpy(outfile2, body);
    strcat(outfile2, "2");
    strcat(outfile2, ".bmp");

    char outfile3[100];
    strcpy(outfile3, body);
    strcat(outfile3, "3");
    strcat(outfile3, ".bmp");


    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }


    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // open output file1
    FILE* outptr1 = fopen(outfile1, "w");
    if (outptr1 == NULL)
    {
        fclose(inptr);
        fclose(outptr);
        fprintf(stderr, "Could not create %s.\n", outfile1);
        return 3;
    }

    // open output file2
    FILE* outptr2 = fopen(outfile2, "w");
    if (outptr2 == NULL)
    {
        fclose(inptr);
        fclose(outptr);
        fclose(outptr1);
        fprintf(stderr, "Could not create %s.\n", outfile2);
        return 3;
    }
    // open output file3
    FILE* outptr3 = fopen(outfile3, "w");
    if (outptr3 == NULL)
    {
        fclose(inptr);
        fclose(outptr);
        fclose(outptr1);
        fclose(outptr2);
        fprintf(stderr, "Could not create %s.\n", outfile3);
        return 3;
    }


    // open log - I want a printout of the Pixels
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        fclose(inptr);
        fclose(outptr);
        fclose(outptr1);
        fclose(outptr2);
        fclose(outptr3);
        fprintf(stderr, "Could not create log.txt\n");
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
        fclose(outptr1);
        fclose(outptr2);
        fclose(outptr3);
        fclose(file);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr1);
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr2);
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr3);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr1);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr2);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr3);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple, triple1, triple2, triple3;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            triple1.rgbtBlue = triple.rgbtBlue; 
            triple2.rgbtBlue = triple.rgbtBlue; 
            triple3.rgbtBlue = triple.rgbtBlue; 

            triple1.rgbtGreen = triple.rgbtGreen; 
            triple2.rgbtGreen = triple.rgbtGreen; 
            triple3.rgbtGreen = triple.rgbtGreen; 

            triple1.rgbtRed = triple.rgbtRed; 
            triple2.rgbtRed = triple.rgbtRed; 
            triple3.rgbtRed = triple.rgbtRed; 


            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);  // The original

            //  This is the point to change a pixel colour and write for triple 1
            ChangePixel(&triple1, 0, 0); // Remove All Blue - equivalent to blue screen
            fwrite(&triple1, sizeof(RGBTRIPLE), 1, outptr1);  // The triple1

            //  This is the point to change a pixel colour and write for triple 2
            ChangePixel(&triple2, 1, 0); // change green
            fwrite(&triple2, sizeof(RGBTRIPLE), 1, outptr2);  // The triple2


            //  This is the point to change a pixel colour and write for triple 3
            // remove all blue and green
            ChangePixel(&triple3, 0, 0); // change blue
            ChangePixel(&triple3, 1, 0); // change green
            fwrite(&triple3, sizeof(RGBTRIPLE), 1, outptr3);  // The triple3

            // write the individual bytes in hex to the log file:
            fprintf(file, "%02x %02x %02x   ", triple3.rgbtBlue, triple3.rgbtGreen, triple3.rgbtRed);
        }


        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // return the cursor
        fprintf(file, "\n");

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
            fputc(0x00, outptr1);
            fputc(0x00, outptr2);
            fputc(0x00, outptr3);
        }
    }

    // close infile
    fclose(inptr);

    // close outfile (s)
    fclose(outptr);
    fclose(outptr1);
    fclose(outptr2);
    fclose(outptr3);

    // close log.txt
    fclose(file);

    // that's all folks
    return 0;
}
void ChangePixel(RGBTRIPLE* Pixel, int Colour, int Value)
{
    switch(Colour) {
        case(0) : 
            Pixel->rgbtBlue = Value;
            break;
        case(1) : 
            Pixel->rgbtGreen = Value;
            break;
        case(2) : 
            Pixel->rgbtRed = Value;
            break;
    }
    
    return;
}

/**
 *  trial.c
 */


/**
 *****************************************
 *      Libray Initialization
 *****************************************
 */
#include <stdio.h>
#include <cs50.h>
#include <string.h>


// Initialize Functions //
#include "bmp.h"
/**
 *****************************************
 *
 *       Main Code
 *
 *****************************************
 */

const int MAX = 3;

int main () {


    RGBTRIPLE triple;
    unsigned char *pointer =(unsigned char*) &triple;

    triple.rgbtBlue = 15; 
    triple.rgbtGreen = 150; 
    triple.rgbtRed = 255;

   /* let us have array address in pointer */

    printf("%02x %02x %02x   \n", triple.rgbtBlue, triple.rgbtGreen, triple.rgbtRed);
    printf("%02x %02x %02x   \n", (int) &triple.rgbtBlue, (int) &triple.rgbtGreen, (int) &triple.rgbtRed);
	
    for (int i = 0; i < sizeof(triple); i++) {



        printf("triple[%d] = %x\n", i, (int) &pointer[i]);
//        BYTE *tempPixelPointer = &tempPointer;
//        printf("tempPixel[%d] = %hhu\n", i, tempPixel );
        printf("Pixel[%d] = %02x\n", i, pointer[i] );
//        printf("tempPixel[%d] = %i\n", i, tempPixel );

        /* move to the next location */
//        pointer = pointer + 1;
    }

    int a = 3;
    unsigned char *p = (unsigned char *)&a;
    int i;

    printf("a =");
    for (i = 0; i < sizeof(a); ++i)
    {
        printf(" %02x", p[i]);
    }
    printf("\n");
 
 
    char x ='a';
    char y ='z';
    char diff;
    
    diff = y - x;
    printf("diff as Ascii = %d\n", diff);
    printf("diff as Character = %c\n", diff);
    
    int idiff = (int) diff;
    printf("idiff as integer = %i\n", idiff);
 
	
    return 0;
}

// do some pointer maths.
//    double ww = 0.5;
//    outpointer[0] = inpointer[0] - 0x7f;
//    outpointer[1] = ww * inpointer[1];
//    printf("outpointer[0] = %02x  ", outpointer[0]);
//    printf("outpointer[1] = %02x  ", outpointer[1]);
//    printf("\n");
//    while (!feof(inptr)) // Find the first jpeg
//    {

        // check for first part of signature
//        if ((byte[0] == 0xff) && (byte[1] == 0xd8) && (byte[2] == 0xff) && (byte[3] >= 0xe0 && byte[3] <= 0xef))
//        {
            //  this is a jpeg - do something
//            njpeg++;
//            printf("found a jpeg %i at byte no %i:  %02x %02x %02x %02x \n", njpeg, nbytes, byte[0], byte[1], byte[2], byte[3]);
//            fread(jpegbuffer, FAT - BytesToRead, 1, inptr);
//            nbytes += (FAT - BytesToRead);
//        } else
//        {
            // move back three bytes
//            fseek(inptr, -3, SEEK_CUR);
            
//        }
//        fread(byte, BytesToRead, 1, inptr);
//        nbytes++;
//    }

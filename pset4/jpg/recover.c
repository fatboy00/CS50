/**
 * recover.c
 * this is actually hacker version
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prototypes
int isjpeg();

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 1)
    {
        printf("Usage: ./recover\n");
        return 1;
    }

    // filename for input file
    char* infile = "card.raw";
    // allocate pointer for output file
    char* outfile = malloc(strlen("000.jpg") + 1);

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }
    FILE* outptr;


    
    int BytesToRead = 4;
    int FAT = 512;
    unsigned char* byte = malloc(BytesToRead);
    unsigned char signature[BytesToRead + 1];
    int njpeg = 0;
    unsigned char* jpegbuffer = malloc(FAT - BytesToRead);
    signature[0] = 0xff;
    signature[1] = 0xd8;
    signature[2] = 0xff;
    signature[3] = 0xe0;
    signature[4] = 0xef;
    
    
    fread(byte, BytesToRead, 1, inptr);
    while (njpeg < 1) // Find the first jpeg
    {

        // check for first part of signature
        if (isjpeg(byte, signature))
        {
            //  this is a jpeg - do something
            sprintf(outfile, "%03d.jpg", njpeg);
            njpeg++;

            // read the rest of the block
            fread(jpegbuffer, FAT - BytesToRead, 1, inptr);
            // open first jpeg file and write first block to it including the first four bytes
            // open new Jpeg file
            outptr = fopen(outfile, "w+");
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", outfile);
                return 3;
            }
            
            // write the buffer and bytes to the jpeg
            fwrite(byte, BytesToRead, 1, outptr);
            fwrite(jpegbuffer, FAT - BytesToRead, 1, outptr);
            fread(byte, BytesToRead, 1, inptr);    // read first four bytes of next block
        } else
        {
            // move back three bytes
            fseek(inptr, -3, SEEK_CUR);
            // check the next sequence for signature
            fread(byte, BytesToRead, 1, inptr);
        }
    }
    
    while(!feof(inptr)) // now read to end of file
    {
        if (isjpeg(byte, signature))
        {
            //  close previous jpeg file
            fclose(outptr);  

            sprintf(outfile, "%03d.jpg", njpeg);
            njpeg++;

            // read the rest of the block
            fread(jpegbuffer, FAT - BytesToRead, 1, inptr);
            // open first jpeg file and write first block to it including the first four bytes
            // open new Jpeg file
            outptr = fopen(outfile, "w+");
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", outfile);
                return 3;
            }
            
            // write the buffer and bytes to the jpeg
            fwrite(byte, BytesToRead, 1, outptr);
            fwrite(jpegbuffer, FAT - BytesToRead, 1, outptr);
            fread(byte, BytesToRead, 1, inptr);    // read first four bytes of next block
        } else
        {
            // read remainder of block
            fread(jpegbuffer, FAT - BytesToRead, 1, inptr);
            // write to existing jpeg including first four bytes
            fwrite(byte, BytesToRead, 1, outptr);
            fwrite(jpegbuffer, FAT - BytesToRead, 1, outptr);
            // read first four bytes of next block 
            fread(byte, BytesToRead, 1, inptr);
        }
    }

    // free allocated memory
    free(byte);
    free(jpegbuffer);
    free(outfile);
    // close infile
    fclose(inptr);
    fclose(outptr);  

    //  the end!
    return 0;


}
int isjpeg(unsigned char* byte, unsigned char* signature) // must use unsigned as signed takes up more bytes
{
    if ((byte[0] == signature[0]) && (byte[1] == signature[1]) && (byte[2] == signature[2]) && (byte[3] >= signature[3] && byte[3] <= signature[4]))
    {
        return 1;   
    } else
    {
        return 0;
    }
}

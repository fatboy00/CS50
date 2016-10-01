/**
 ********************************************************************************
 *      argvtest Program                                                        *
 *                                                                              *
 *      Finds error message if no argv                                          *
 *                                                                              *
 ********************************************************************************
 */


/**
 *****************************************
 *      Libray Initialization
 *****************************************
 */
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>


// Initialize Functions //
// None 

/**
 *****************************************
 *
 *       Main Code
 *
 *****************************************
 */


int main(int argc, char* argv[])
{
    int errorReturn = 0;

    if (argc == 2) {
        errorReturn = 0;
        printf(" correct arg entered \n");
    
         // print arguments
        for (int i = 0; i < argc; i++)
        {
            for (int j = 0, n = strlen(argv[i]); j < n; j++)
            {
                printf("%c\n", argv[i][j]);
            }
            printf("\n");
        }
    }
    else {
        errorReturn =1;
    }

    return errorReturn;
    
}

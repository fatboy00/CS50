/**
 ********************************************************************************
 *      File Writing Program                                                    *
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
    int i;
	
		i = 0;
		while(i++ < 5) {
			printf("%d\n", i);
		}
		printf("\n");
		i = 0;
		while(++i < 5) {
			printf("%d\n", i);
		}
		return 0;
    
 }

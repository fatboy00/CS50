/**
 ********************************************************************************
 *      Ceaser Program                                                          *
 *                                                                              *
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
    char* ClearText;
    char* CypherText;
    int errorReturn = 0;
    int TextLength = 0;
    int KeyShift = 0;

    // is argument the correct size? //
    if (argc != 2) {
        errorReturn = 1;
        printf(" put an argument in please \n");
    }

    if (errorReturn == 0) {
        KeyShift = atoi (argv[1]);  // The Ceaser Shift
        if (KeyShift < 0) {
            errorReturn = 1;
            printf(" put a positive argument in please \n");
        }    
        
    }
    
    
    if (errorReturn == 0 ) {


        ClearText = GetString();
        CypherText = ClearText;
        TextLength = strlen(ClearText);  //  Number of Characters to Cycle through
        // now reduce to a number less than 26!
        KeyShift = KeyShift % 26;

        
        // shift according to Cypher        
        for ( int i = 0; i < TextLength; i++){
            int Diff = 0;

            if (ClearText[i] > 64 && ClearText[i] < 91) {
             
                //  do the shift for UPPER case//
                if (ClearText[i] + KeyShift > 90) {
                    Diff = ClearText[i] + KeyShift - 90;
                    CypherText[i] = Diff + 64;
                }
                else {
                    CypherText[i] = ClearText[i] + KeyShift;
                }
            }
            else if (ClearText[i] > 96 && ClearText[i] < 123 ) {
                
                //  do the shift for lower case//
                if (ClearText[i] + KeyShift > 122) {
                    Diff = ClearText[i] + KeyShift - 122;
                    CypherText[i] = Diff + 96;
                }
                else {
                    CypherText[i] = ClearText[i] + KeyShift;
                }
            }

            
        }
        printf("%s\n", CypherText);
        
        

    }

    
    
    
    
    return errorReturn;
}

/**
 *****************************************
 *      xxx Function
 *      Finds spaces and builds Initials
 *****************************************
 */



        //  Work out KeyShift //
//        for (int j = 0, n = strlen(argv[1]); j < n; j++)
//        {
//            // remember to subtract 48 for Ascii Value of char!! //
//            KeyShift = KeyShift + ((argv[1][j]-48) * (int) pow (10, (n - j - 1)));
//            printf(" KeyShift = %i\n", KeyShift);
//        }

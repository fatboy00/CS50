/**
 ********************************************************************************
 *      Vigenere Program                                                        *
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
    char* KeyWord;
    int errorReturn = 0;
    int TextLength = 0;
    int KeyLength = 0;
    int KeyCounter = 0;
    int KeyShift[50];

    // is argument the correct size? //
    if (argc != 2) {
        errorReturn = 1;
        printf(" put an argument in please \n");
    }

    if (errorReturn == 0 ) {
        KeyWord = argv[1];
        KeyLength = strlen(KeyWord);  //  KeyWord Length

        for (int i = 0; i < KeyLength; i++) {
            
            //  Is the next character a Letter?
            if (KeyWord[i] > 64 && KeyWord[i] < 91) {
                KeyShift[i] = (int) KeyWord[i] - 65;
            }
            else if (KeyWord[i] > 96 && KeyWord[i] < 123 ) {
                KeyShift[i] = (int) KeyWord[i] - 97;
            }
            else {
                // error to be returned - not a letter
                errorReturn = 1;
                printf(" argument contains non letters \n");
            }
        }
    }
        



    if (errorReturn == 0 ) {

        // Get all the Inputs
        ClearText = GetString();
        TextLength = strlen(ClearText);  //  Number of Characters to Cycle through
        char CypherText[TextLength];

        for (int i = 0; i < TextLength; i++) {
            int Diff = 0;
            //  check if need to reset KeyCounter
            if (KeyCounter > KeyLength - 1 ){
                KeyCounter = 0;
            }
            
            //  Is the next character a Letter?
            if (ClearText[i] > 64 && ClearText[i] < 91) { // do the shift for UPPER case//
                if (ClearText[i] + KeyShift[KeyCounter] > 90) {
                    Diff = ClearText[i] + KeyShift[KeyCounter] - 90;
                    CypherText[i] = Diff + 64;
                    KeyCounter++;
                }
                else {
                    CypherText[i] = ClearText[i] + KeyShift[KeyCounter];
                    KeyCounter++;
                }
            }
            else if (ClearText[i] > 96 && ClearText[i] < 123 ) { //  do the shift for lower case//
                if (ClearText[i] + KeyShift[KeyCounter] > 122) {
                    Diff = ClearText[i] + KeyShift[KeyCounter] - 122;
                    CypherText[i] = Diff + 96;
                    KeyCounter++;
                }
                else {
                    CypherText[i] = ClearText[i] + KeyShift[KeyCounter];
                    KeyCounter++;
                }
            }
            else {
                CypherText[i] = ClearText[i]; // No Shift
            }

        printf("%c", CypherText[i]);

        }

        printf("\n");

    } 
 
 
 
    return errorReturn;
     
 }

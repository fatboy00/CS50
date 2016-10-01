/**
 ********************************************************************************
 *      Password Cracking Program                                               *
 *                                                                              *
 ********************************************************************************
 */


/**
 *****************************************
 *      Libray Initialization
 *****************************************
 */
#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>
#include <time.h>


// Initialize Functions //
int CheckDictionary();
int BruteForce();

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
    int Solved = 0;
    
    
    // is argument the correct size? //
    if (argc != 2) {
        errorReturn = 1;
        printf(" put an argument in please \n");
    }
    
    if (errorReturn == 0) {
        
    // Assign Hash from argv[1]
        string Hash = argv[1];
        char Salt[] ="XY";

    // Find Salt (First Two Characters of argv[1])
        Salt[0] = argv[1][0];
        Salt[1] = argv[1][1];
        
    // Check Dictionary for Hash Matches
        Solved = CheckDictionary(Hash, Salt,"/usr/share/dict/words");
    
    // Now Brute Force Solve
        if ( Solved == 0){
            Solved = BruteForce(Hash, Salt, 8);
        }
    // No Solution Found;
        if ( Solved == 0){
            printf("Not Solved\n");
        }


    }
    
    
    return errorReturn;
     
 }


/**
 *****************************************
 *      CheckDictionary
 *      Checks Dictionary Word Hashes
 *****************************************
 */
int CheckDictionary(char* Hash, char* Salt, char* FileDir)
{
    FILE* DictFile;
    char FileRec[255];
    char Temp[255];
    int Solved = 2;
    char* EOFile = NULL;




    // Open the File    
    DictFile = fopen(FileDir, "r");

    // Loop though the dictionary while not solved
    while ( Solved == 2) {
        fscanf(DictFile, "%s", FileRec);
        EOFile = fgets(Temp, 255, (FILE*) DictFile); // Check if end of file is reached
        if (EOFile != NULL) { // If not, does the encyption match the Hash?
            if (strcmp(Hash, crypt(FileRec, Salt)) == 0) { 
                Solved = 1;
                printf("%s\n", FileRec);
            }
        }
        else {
            Solved = 0;
        }
    }
    fclose(DictFile);
    return Solved;
}

/**
 *****************************************
 *      BruteForce
 *      Generates Printable ASCII Hashes
 *      up to Max Charactes
 *****************************************
 */
int BruteForce(char* Hash, char* Salt, int MaxChar)
{
    int Solved = 2;
    char BruteString[MaxChar+1];
    int CharCounter[MaxChar];
    int MaxASCII = 127; // 127
    int MinASCII = 32; // 32
    clock_t begin, end;
    double time_spent;
    
    // Initialize various Counters
    int CharIndex = 0;       
	for (int i = 0; i < MaxChar;i++)
	{
		CharCounter[i] = 0;		// Initialize all Char to null.
		BruteString[i+1] = 0;   // Null terminate the string.
	}
	CharCounter[0] = MinASCII;			// Initialize the first counter to first trial number
    
    begin = clock();
    while (Solved == 2) {

        BruteString[CharIndex] = CharCounter[CharIndex]; //Shift to the Next Character
        if (strcmp(Hash, crypt(BruteString, Salt)) == 0) { // Check if there is a match
            Solved = 1;
            printf("\n");
            printf("%s\n", BruteString);
            end = clock();
            time_spent = (double) (end - begin)/CLOCKS_PER_SEC;
            printf("Time Spent = %f\n", time_spent);
        }
        
        CharIndex =0;  // Start with First Character of BruteForce string
        CharCounter[CharIndex]++; // Increment to next Character
        while (CharCounter[CharIndex] > MaxASCII && Solved == 2) {

            CharCounter[CharIndex] = MinASCII; // Reset the Character to Beginning
            BruteString[CharIndex] = MinASCII; // and in The Test String
            CharIndex++; // Move to Next Charachter to Right

            if (CharCounter[CharIndex] == 0){
                CharCounter[CharIndex] = MinASCII; // Set the first Character
                end = clock();
                time_spent = (double) (end - begin)/CLOCKS_PER_SEC;
                printf("Time Spent on %i char length string = %f sec\n", CharIndex, time_spent);
                printf("Checking %i char length Strings : ", CharIndex + 1);
                printf("estimated time is %f sec\n",(MaxASCII-MinASCII) * time_spent );
                begin = end;
            } else {
                CharCounter[CharIndex]++; // Increment to next Character
                
            }
            
            if (CharIndex > MaxChar-1) { // Have You run out of Characters?
                Solved = 0; // Solution Not Found
            }
        }

    }
    return Solved;

}


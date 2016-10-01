/**
 ********************************************************************************
 *      Initials Program                                                        *
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


// Initialize Functions //
string GetName();
string PrintInitials();

/**
 *****************************************
 *
 *       Main Code
 *
 *****************************************
 */

int main(void)
{
    string Name = "";
    string Initials = "";
    Name = GetName();
    Initials = PrintInitials (Name);
    printf("%s\n", Initials); 
}



/**
 *****************************************
 *      GetName Function
 *      Checks for Non String Input 
 *****************************************
 */
string GetName(Void)
{
    bool Complete;
    char* Name;
    int nSpaces;
    
    // check for all ASCII characters and spaces //
    do {
//        printf("Enter Name ");
        Complete = false;
        nSpaces = 0;
        Name = GetString();    
        int Length = strlen(Name);

        // no spaces at start / end - fix if necessary to drop//
        if (Name[Length-1] == 32) {
            Complete = true;
            printf("drop space at end of name");    
        }
        if (Name[0] == 32) {
            Complete = true;
            printf("drop space at start of name");    
        }
        if (Name == NULL) {
            Complete = true;
            printf("enter a name");    
        }


        //  Check only letters or spaces //
        for (int i =0; i < Length; i++) {
            if (Name [i] != 32 && !(Name[i] > 64 && Name[i] < 91) && !(Name[i] > 96 && Name[i] < 123 )) {
                Complete = true;
                printf("Non Letter Characters included");    
            }
            // single spaces between names only  - fix if necessary to concatenate//
            if (Name[i] == 32 && Name [i-1] == 32) {
                Complete = true;
                printf("More than one space between names");    
            }
        }

    } while (Complete);
    return Name;
}

/**
 *****************************************
 *      GetInitials Function
 *      Finds spaces and builds Initials
 *****************************************
 */
string PrintInitials(string Name)
{

    int Length = strlen(Name);
    char Dest[100];
    char Src[] = " ";
    int Flag = 1;



    for (int i = 0; i < Length ; i++) {

        if (Flag == 1 && Name [i] != 32) {
            if (Name[i] > 96 && Name[i] < 123 ) {
                Src[0] = (char) Name[i] - 32;
            }
            else {
                Src[0] = (char) Name[i];
            }

            if  (i == 0 && Name[i] != 32) {
                strcpy(Dest, Src);
            }
            else {
                strcat(Dest , Src);
            }
            Flag = 0;
        }
        if ( Name [i] == 32) {
            Flag = 1;
        }

    }

    char* Initials = Dest;
//    printf("Initials are %s\n", Initials); 

    return Initials;
}

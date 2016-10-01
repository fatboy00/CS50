/**
 * godmode.c
 *
 */

#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>
#include "godmode.h"

// constants
#define DIM_MIN 3
#define DIM_MAX 9
#define MAX_PATHS 50
#define LIMIT 65536


//  try out a structure here
struct ListStruct {
    int Layout[DIM_MAX * DIM_MAX];  // Actual board Layout for this element
    int BlankTile; // Location of blank tile to save calculation time
    int ManhattenCost; // Manahtten Heuerisitic Calculation
    int PathCost; // Number of Rounds to get to this board Layout
    int TotalCost; // the Cost so far, plus the heureistic cost
    int OpenOrClosed; // have we already derived the child boards?
    char* Path; // actual path to get here.  Assumes max 100 elements!
    struct ListStruct* NextList; // Pointer to Previous List!  This way you dont need to remember any array number!
};

int manhatten();
struct ListStruct* CreateNewElement();
struct ListStruct* LeastCostElement();
int FillLayout();
bool won();
bool CanMove();
bool BoardExists();
char* SolveIt();


/**
 * Returns an Integer or enables godmode or asks for retry
 */
void GetMove(int *intOrString)
{

    int tempInt = 0;
    int strLength = 0;
    bool acceptEntry = false;

    while (!acceptEntry) {
        string tempString = GetString(); // get keyboard input
        strLength = strlen(tempString);
        bool isInt = true;

        if (strLength == 0) {
            isInt = false;
        }

        int i = 0; // check if int or contains strings
        while (i < strLength && isInt == true) {
            if (tempString[i] >= '0' && tempString[i] <= '9') { // Prevents negative entries
                isInt = true;    
            } else {
                isInt = false;
            }
            i++;
        }
        
        if (isInt) {
            acceptEntry = true;
            tempInt = atoi(tempString);
            *intOrString = tempInt;
        } else {
            if (strcmp(tempString ,"GOD") == 0 ) {
                tempInt = -99;
                *intOrString = tempInt;
                acceptEntry = true;
            } else {
                printf("Retry: ");
                acceptEntry = false;
            }
        }
    }
    return;
}
/**
 * Determine target board 
 */
void calctarget(int n, int target[][n])
{
    // This is automatically calculated 
    // but could be rplaced with user entry if desired 
    int nPieces = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (nPieces == n * n) {
                target[i][j] = 0;
            } else {
                target[i][j] = nPieces;
            nPieces++;
            }
        }
    }
    return;
}
/**
 * Here is the Solver as a function.  Will return Path if solvable!
 * 
 */
char* SolveIt(int d, int board[], int target[])
{
    // put the starting board into the List Structure
    struct ListStruct* InitialElement = CreateNewElement();  // create the first element
    InitialElement->BlankTile = FillLayout(d, (int*) board, InitialElement); // place tiles on board and get blank tile position
    InitialElement->ManhattenCost = manhatten(d, (int*) target, (int*) InitialElement->Layout);
    InitialElement->TotalCost = InitialElement->PathCost + InitialElement->ManhattenCost;
    
    // create a Pointer that points to the last element that was initialized in the queue of Lists;
    struct ListStruct* LastElementInitialized = InitialElement;

    // Initializethe next Node
    struct ListStruct* NewNode = CreateNewElement();  // create a new Node for first run through



    //
    // This is where the search takes place
    //
    // Figure out the next possible board positions  and iterate through:
    bool iterate = true;
    int trial = 0;
    string clearline = "                                              ";
    while (iterate) 
    {
        // find the least cost element from the open nodes
        struct ListStruct* Node = LeastCostElement(LastElementInitialized);  // find the least cost element
        printf("\rTrial %i, Path = %s %s", trial++, Node->Path, clearline);

        
        if (Node == NULL) { // No Nodes left in queue!
            printf("No Solution Found!");
            iterate = false;
            return "No Solution Found!";
            break;
        }


        // does the board match the target?
        if (won(d, (int*) Node -> Layout, (int*) target)) {
            iterate = false;
            return Node-> Path;
            break;
        }

        // now make the move(s) and add to the queue (if the moves are legal)...
        for (int PossibleMoves = 0; PossibleMoves <= 3; PossibleMoves++) {
            if (CanMove(d, (int*) Node -> Layout, PossibleMoves, Node -> BlankTile, (int*) NewNode -> Layout, &NewNode -> BlankTile)) { // is the move legal?
                // check if the board already exist?
                if (!BoardExists(d, (int*) NewNode -> Layout, LastElementInitialized)) { // check we haven't created this board before
                    // and add the missing data (e.g. cost etc)
                    NewNode->ManhattenCost = manhatten(d, (int*) target, (int*) NewNode->Layout);
                    NewNode->PathCost = Node->PathCost + 1; // increment the path cost
                    NewNode->TotalCost = NewNode->PathCost + NewNode->ManhattenCost;
                    strcpy(NewNode->Path, Node->Path);
                    NewNode->Path[Node->PathCost] = PossibleMoves + '0'; //Replace the X with the actual move
                    NewNode->Path[NewNode->PathCost]= 'X'; // add a new 'X'
                    NewNode->NextList = LastElementInitialized;
                    LastElementInitialized = NewNode;
                    NewNode = CreateNewElement();  // create a new Node for next run through
                    if (NewNode == NULL) {
                        printf("Out of Memory!");
                        iterate = false;
                        return "Out of memory!";
                        break;
                    }
                }
            }
        }
    }
    
    
    

    return "Should not be here!";
}

/**
 * calculate manhatten distance (Heueristic)
 * 
 */
int manhatten (int d, int target[], int compare [])
{
    int sum = 0;
    int elementtarget, elementcompare;
    int tempRow, tempCol;


    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            elementtarget = (i * d) + j; // calculate position on flattened matrix
            // now search for that in board layout (if board value = 0 it will not be found and thus no manhatten value calculated)
            for (int k = 0; k < d; k++) {
                for (int l = 0; l < d; l++) {
                    elementcompare = (k * d) +l;
                    if (compare[elementcompare] == target[elementtarget] && compare[elementcompare]) {
                        tempRow = abs(k - i);                        
                        tempCol = abs(l - j);                        
                        sum = sum + tempRow + tempCol;
                    }
                }
            }
            // next value
        }
    }
   
    return sum;
}
/**
 * Create a new element that has board state etc., 
 * 
 */
 struct ListStruct* CreateNewElement()
 {
    struct ListStruct* temp;
    temp = (struct ListStruct*) malloc(sizeof(struct ListStruct)); //malloc/Calloc?
    if (temp == NULL) 
    {
        return NULL;
    }
    temp -> Path = malloc(sizeof(char) * MAX_PATHS);     
    temp -> Path[0]= 'X'; // Initialized Path is 'X'
    temp -> OpenOrClosed = 1;
    temp -> PathCost = temp -> TotalCost = 0;
    temp -> NextList = NULL;
    return temp;
 }

/**
 * Fill the Layout with the board position and find the BlankTile 
 * 
 */
 int FillLayout(int d, int board[], struct ListStruct* ListPtr)
 {
    int BlankPos = -1;
    for (int i = 0; i < d * d; i++) {
        ListPtr -> Layout[i] = board[i];
        if (board[i] == 0) {
            BlankPos = i;
        }
    }
    return BlankPos;
 }
 
/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(int d, int board[], int target[])
{
    // check 

    for (int i = 0; i < d * d; i++) {
        if (board[i] != target[i]) {
                return false;
        }
    }
    return true;
}
/**
 * Test and make moves on block 
 * 
 */
bool CanMove(int d, int board[], int PossibleMoves, int BlankTile, int newboard[], int* NewBlankTile) 
{
    bool DoMove = true;
    for (int i = 0; i < d ; i++) {
        switch (PossibleMoves) {
            case 0: // North Move for space
                if(i == BlankTile) {
                    DoMove = false;
                }
                break;
            case 1: // East Move for space
                if((i+1)*d - 1 == BlankTile) {
                    DoMove = false;
                }
                break;
            case 2: // South Move for space
                if(d*d - i - 1 == BlankTile) {
                    DoMove = false;
                }
                break;
            case 3: // West Move for space
                if(i*d == BlankTile) {
                    DoMove = false;
                }
                break;
            default: ;
        }
    }
    
    if (DoMove) {
        for (int i = 0; i < d * d ; i++) {
            newboard[i] = board[i];
        }
        switch (PossibleMoves) {
            case 0: // North Move for space
                newboard[BlankTile] = board[BlankTile - d];
                newboard[BlankTile - d] = 0;
                *NewBlankTile = BlankTile - d;
                break;
            case 1: // East Move for space
                newboard[BlankTile] = board[BlankTile + 1];
                newboard[BlankTile + 1] = 0;
                *NewBlankTile = BlankTile + 1;
                break;
            case 2: // South Move for space
                newboard[BlankTile] = board[BlankTile + d];
                newboard[BlankTile + d] = 0;
                *NewBlankTile = BlankTile + d;
                break;
            case 3: // West Move for space
                newboard[BlankTile] = board[BlankTile - 1];
                newboard[BlankTile - 1] = 0;
                *NewBlankTile = BlankTile - 1;
                break;
            default: ;
        }
    }
    return DoMove;

}
/**
 * Find the next leastcost element
 * 
 */
struct ListStruct* LeastCostElement(struct ListStruct* ListPointer)
{
    struct ListStruct* Cheapest = NULL;
    int MinTotalCost = 100;
    
    while (ListPointer != NULL) {
        if (ListPointer->TotalCost < MinTotalCost && ListPointer->OpenOrClosed) {
            Cheapest = ListPointer;
            MinTotalCost = ListPointer->TotalCost;
        }
        ListPointer = ListPointer->NextList; // go to the next element
    }
    if (Cheapest != NULL) {
        Cheapest->OpenOrClosed = 0;
    }
    return Cheapest;
}
/**
 * Check the board has not been created before
 * 
 */
bool BoardExists(int d, int board[], struct ListStruct* ListPointer)
 {
    int i;
    while (ListPointer != NULL) {
        for (i = 0; i < d * d ; i++) {
            if ( ListPointer->Layout[i] != board[i]) {
                break;
            }
        }
        if (i == d * d) {  // this one is identical!  so no need to record board!
            return true;
        }
        ListPointer = ListPointer->NextList; // go to the next element
    }
    return false;
 }
/**
 * Convert space moves (0= Up, 1 = Right, 2= down, 3= left)
 * into tile names so that it works with pset3 logic.
 */
int GetGodModeMove(int d, int board[][d],int BlankRow, int BlankColumn, int NextMove) 
{

    int tile = 0;
    switch (NextMove) {
        case 0: // North Move for space
            tile = board[BlankRow - 1][BlankColumn];
            break;
        case 1: // East Move for space
            tile = board[BlankRow][BlankColumn + 1];
            break;
        case 2: // South Move for space
            tile = board[BlankRow + 1][BlankColumn];
            break;
        case 3: // West Move for space
            tile = board[BlankRow][BlankColumn - 1];
            break;
        default: ;
    }

    return tile;
}
/**
 * shuffle a pre-entered board
 * by generating random numbers to represent up/down/left right moves
 * 
 */
void shuffle(int d, int board[][d], int* BlankRow, int* BlankColumn)
{
    int NoOfShuffle;
    int Move = -1;
    int PreviousMove = -1;
    int Random;
    int BlankTile = -1;
    int newboard[d][d];
    int NewBlankTile = -1;
    int i = 0;
    int NoIterations = 0;
    
    if (d == 3) {
        NoOfShuffle = 10;
    } else {
        NoOfShuffle = 20;
    }
    
    srand48((long int) time(NULL));
    BlankTile = (*BlankRow * d) + *BlankColumn;

    // print out the requisite number of random numbers
    while (i < NoOfShuffle && NoIterations < 1000)
    {
        NoIterations++;
        Random = (int) (drand48() * LIMIT);

        if (Random >= 0 && Random < 25000 ) {
            Move = 0;
        } else if (Random >= 25000 && Random < 50000 ) {
            Move = 1;
        } else if (Random >= 50000 && Random < 75000 ) {
            Move = 2;
        } else if (Random >= 75000 && Random < 100000 ) {
            Move = 3;
        } else {
            printf("weird Random occured = %i", Random);
        }    
        if (Move == PreviousMove & Move <3) {
            Move = Move + 1;
        } else if (Move == PreviousMove & Move <3) {
            Move = 0;
        }
        
        if (CanMove(d, (int*) board, Move, BlankTile, (int*) newboard, &NewBlankTile)) {
            BlankTile = NewBlankTile;
            for (int j = 0; j < d; j++) {
                for (int k = 0; k < d; k++) {
                    board[j][k] = newboard[j][k];
                    //  get blankrow and column reset!
                    if (board[j][k] == 0) {
                        *BlankRow = j;
                        *BlankColumn = k;
                    }
                }
            }
            PreviousMove = Move + 2;
            if (PreviousMove > 3) {
                PreviousMove = PreviousMove - 4;
            }
            i++;
        }
    }
    return;
}


/**
 * Copy 2D Arrays of different dimensions.  
 * Fill Array with zeros if Destination (dest) Array is bigger that Source (srce) Array
 * 
 */
//void copy(int ns, int nd, int srce[][ns],int dest[][nd])
//{
//    for (int i = 0; i< nd; i++) {
//        for ( int j = 0;j< nd;j++) {
//            dest[i][j]=srce[i][j];
//            if ( (j >= ns) || (i >= ns)) {
//                dest[i][j] = 0;
//            }
//        }
//    }
//}

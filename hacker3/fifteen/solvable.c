// C++ program to check if a given instance of 8 puzzle is solvable or not
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9
#define MAX_PATHS 50



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




bool isSolvable();
int getInvCount();
void draw();
int manhatten();
struct ListStruct* CreateNewElement();
struct ListStruct* LeastCostElement();
int FillLayout();
bool won();
bool CanMove();
bool BoardExists();
char* SolveIt();



/* Driver progra to test above functions */
int main()
{
    int d = 4;
    char* Sequence;
    
    int board[4][4] =  {{1, 2, 3, 4},
                      {5, 15, 14, 12},  // Value 0 is used for empty space
                      {9, 11, 10, 8},
                      {13, 7, 6, 0} };


    int target[4][4] =  {{1, 2, 3, 4},
                      {5, 6, 7, 8},  // Value 0 is used for empty space
                      {9, 10, 11, 12},
                      {13, 14, 15, 0} };

    if (isSolvable(board)) {
          printf(" Solvable\n" );
        
    } else {
          printf(" Not Solvable\n" );
          return 2;
    }

    Sequence = SolveIt(d, (int*) board, (int*) target);
    printf("%s\n",Sequence);
    
    

  return 0;
}
// A utility function to count inversions in given array 'arr[]'
int getInvCount(int arr[])
{
    int inv_count = 0;
    for (int i = 0; i < 9 - 1; i++) {
        for (int j = i+1; j < 9; j++) {
             // Value 0 is used for empty space
             if (arr[j] && arr[i] &&  arr[i] > arr[j]) {
                inv_count++;
             }
        }
    }
    return inv_count;
}
 
// This function returns true if given 8 puzzle is solvable.
bool isSolvable(int puzzle[3][3])
{
    // Count inversions in given 8 puzzle
    int invCount = getInvCount((int*) puzzle);
 
    // return true if inversion count is even.
    return (invCount%2 == 0);
}

// This prints out array
void draw(int d, int board[][d])
{
    // Draw lines to separate boards
    printf("---------------------------------------------------------\n");

    // Draw Board - Leave Space between numbers and assume underscore for Blank Tile:
    for (int i = 0; i < d; i++) {
        printf("\n");
        for (int j = 0; j < d; j++) {
            if (board[i][j] > 0) {
                printf("  %2d", board[i][j]);
            } else {
                printf("  __");
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Here is the Solver as a function.  Will return Path
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
    while (iterate) 
    {
        // find the least cost element from the open nodes
        struct ListStruct* Node = LeastCostElement(LastElementInitialized);  // find the least cost element
        if (Node == NULL) {
            printf("No Solution Found!");
            iterate = false;
            return "No Solution Found!";
            break;
            
        }

        // does the board match the target?
        if (won(d, (int*) Node -> Layout, (int*) target)) {
            printf("....Victory is sweet.......\n");
            printf("Path = %s\n", Node->Path);
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
                        return "Out of Memory!";
                        break;
                    }
                }
            }
        }
    }
    
    
    

    return "Should Not be here!";
}

// calculate manhatten distance
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

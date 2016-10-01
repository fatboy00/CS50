/**
 * fifteen.c
 *
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// blanktile location & Surrounding locations(?)
int BlankColumn;
int BlankRow;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{

    // Assign initial numbers to board places
    int nPieces = (d * d) - 1; 
    
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            board[i][j] = nPieces;
            nPieces--;
        }
    }
    // check even or odd number of pieces and swap 1 & 2 if uneven
    nPieces = (d * d) - 1; 
    if (nPieces%2 != 0) {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;

    }
    // assign blank tile location
    BlankColumn = d-1;
    BlankRow = d-1;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
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
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    int tileColumn, tileRow;
    
    //  is the tile legal (within max)
    if (tile >= d * d ) {
        return false;
    }
    if (tile < 0 ) {
        return false;
    }
    //  search for tile
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] == tile) {
                tileColumn = j;
                tileRow = i;
            }
        }
    }
    //  is the tile next to blank tile
    if ((tileRow != BlankRow) && (tileColumn != BlankColumn)) {        // wrong row & wrong column
        return false;
    }
    if (tileRow == BlankRow) { // right row & wrong column
        if ( !((tileColumn == BlankColumn - 1) || (tileColumn == BlankColumn + 1))){
            return false;
        }
    }
    if (tileColumn == BlankColumn) { // wrong row & right column
        if ( !((tileRow == BlankRow - 1) || (tileRow == BlankRow + 1))){
            return false;
        }
    }
    
    //  record location of blank tile using global variable.
    board[BlankRow][BlankColumn] = tile;
    board[tileRow][tileColumn] = 0;
    BlankColumn = tileColumn;
    BlankRow = tileRow;
    
    return true;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // TODO
    int nPieces = 1;
    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            if (board[i][j] != nPieces) {
                if (i < d - 1) {
                    return false;
                } else if (j < d - 1) {
                    return false;
                }
            }
            nPieces++;
        }
    }

    return true;
}

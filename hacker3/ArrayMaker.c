/**
 * ArrayMaker.c
 *
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

 
#define numRows 9
#define numCols 9
#define TotalNum (numRows*numCols)
void display();
void copy();
void resize();
int arr[numRows][numCols] = {{8,7,6}, {5,4,3},{2,1,0}};


int main() {
    display(numCols, arr);
    int d = 3;
    int startArray[d][d];
    copy(numCols, d, arr, startArray);
    display(d, startArray);
}

//  Print the Array
void display(int n, int p[][n])
{
    printf("\n");
    for (int i = 0; i< n;i++)
    {
        for ( int j = 0;j< n;j++)
        {
            printf("%i\t",p[i][j]);
        }
        printf("\n");
    }
}
//  copy the Array
void copy(int ns, int nd, int srce[][ns],int dest[][nd])
{
    for (int i = 0; i< nd; i++) {
        for ( int j = 0;j< nd;j++) {
            dest[i][j]=srce[i][j];
            if ( (j >= ns) || (i >= ns)) {
                dest[i][j] = 0;
            }
        }
    }
}

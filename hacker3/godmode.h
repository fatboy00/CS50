/**
 * godmode.h
 *
 */
 
#include <cs50.h>


/**
 * Returns an Integer or enables godmode flag
 */
void GetMove(int *intOrString);
/**
 * Copy 2D Arrays of different dimensions.  
 * Fill Array with zeros if Destination (dest) Array is bigger that Source (srce) Array
 * 
 */
void copy(int ns, int nd, int srce[][ns],int dest[][nd]);
/**
 * Determine target board 
 */
void calctarget(int n, int target[][n]);

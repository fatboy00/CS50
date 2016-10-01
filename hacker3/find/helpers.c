/**
 * helpers.c
 *
 */
       
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    int min = 0;
    int max = n - 1;
    int midpoint = (min + max) / 2;

    // check that value is inside the sorted range to save time!
    if ((value == values[0]) || (value == values[n-1]) ) {
        return true;
    } else if (value > values[n-1]) {
        return false;
    } else if (value < values[0]) {
        return false;
    }


    while (min < max - 1)  {
        if (value == values[midpoint]) {
            return true;
        } else if (value < values[midpoint]) {
            max = midpoint;
        } else {
            min = midpoint;
        }
        midpoint = (min + max) / 2;
    }

    return false;
    // end linear search
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    // implement bubble sort
    bool DidSwap= true;
    int i = 0;
    int j = 0;
    int SwapValue;
    
    while ((j < n - 1) && (DidSwap = true)) {
        DidSwap = false;
        for (i = 0; i < n - 1 - j; i++) {
            if (values[i] > values[i+1]) {
                DidSwap = true;
                SwapValue = values[i];
                values[i] = values[i+1];
                values[i+1] = SwapValue;
            }
        }
        j++;
    }
    
    
    return;
}

/**
 * Tests a new function called do nothing.
 */
void DoNothing(void)
{
    return;
}

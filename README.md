/**
 * greedy.c
 * 
 */
#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void) {
    
    float total = 0.0;
    int cents = 0;
    int ntwentyfive = 0;
    int nten = 0;
    int nfive = 0;
    int none = 0;
    
    
    // Do while loop //
    do {
        printf(" Enter Change required ");
        total = GetFloat();    
    } while (total < 0);


    // round the amount up or down //
    cents = round (total * 100);
//    printf(" pennies %i\n", cents);

    // check more than 25 cents  //
    if (cents >= 25) {
        // determine number of 25 cent pieces //
        do {
            cents = cents -25;
            ntwentyfive ++;
        } while (cents >= 25);
        
    }
    
    if (cents >= 10) {
        // determine number of 10 cent pieces //
        do {
            cents = cents -10;
            nten ++;
        } while (cents >= 10);
    }
  
    if (cents >= 5) {
        // determine number of 5 cent pieces //
        do {
            cents = cents -5;
            nfive ++;
        } while (cents >= 5);
    }  
  
    if (cents >= 1) {
        // determine number of 1 cent pieces //
        do {
            cents = cents -1;
            none ++;
        } while (cents >= 1);
    }  
  
  
    

//    printf(" coins: %i\n", ntwentyfive + nten + nfive + none);
    printf("%i\n", ntwentyfive + nten + nfive + none);
    

//    printf(" quarters: %i\n", ntwentyfive);
//    printf(" dimes: %i\n", nten);
//    printf(" nickels: %i\n", nfive);
//    printf(" pennies: %i\n", none);
    
}

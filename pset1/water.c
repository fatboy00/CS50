/**
 *      water.c
 */
#include <stdio.h>
#include <cs50.h>

int main(void)
{
        int minutes; 
        int bottles;
        printf("I will calculate your shower length\n");
        printf(" Enter the minutes in Shower: ");
        minutes = GetInt();
        bottles = minutes * 12;
        printf("Bottles %i\n", bottles);
        
}

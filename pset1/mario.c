/**
 *      Mario
 * 
 */

#include <stdio.h>
#include <cs50.h>

int main(void)
{
        int height = 24;
        int y = 1;
        int x = 1;
        int initial = 2;
        int width = 1;
        printf("Pyramid Printout\n");
        
        // Do while loop //
        do {
            printf(" Enter the height of the Pyramid: ");
            height = GetInt();    
        } while ((height < 1) || (height > 23));
        width = height;


        // simple half pyramid //
//        for (y = initial; y < (height + initial) ; y ++) {
//            do {
//                printf ("#");
//                x = x + 1;
//            } while (x < y);
//           printf ("#\n");
//            x = 1;
//        }
    
        // now get steps right way round //
//       printf("Pyramid right way round!\n");
        for (y = initial; y < (height + initial) ; y ++) {
            for ( x = width; x >= initial; x--){
                if ( x < y) {
                    printf ("#");
                }
                else {
                    printf (" ");
                }
                
            }
            printf ("##\n");


        }

}

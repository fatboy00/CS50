/**
 *      credit.c
 * 
 */
#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void) {

        long long CardNumber = 0;
        long long DigitCardNumber = CardNumber;
        bool Input = false;
        int maxDigit = 16;
        int minDigit =13;
        int nDigits;
        string CardMake = "INVALID";
        int DigitArray[maxDigit];
        int CheckSumOdd = 0;
        int CheckSumEven = 0;
        int CheckSumArray[maxDigit];


        // Do while loop //
    do {
        printf(" Enter Number: ");
        CardNumber = GetLongLong();
        DigitCardNumber = CardNumber;
        Input = false;
        CheckSumOdd = 0;
        CheckSumEven = 0;
        CardMake = "INVALID"; 
                
        // is number positive? //
        if ( CardNumber <= 0) {
            Input = true;
            CardNumber = CardNumber * -1;
            printf(" Needs to be greater than zero.  ");
        }
        
        // is length in right range? //
        nDigits = floor(log10(CardNumber)) + 1;
        if ( nDigits < minDigit || nDigits > maxDigit) {
            Input = true;
            printf(" Needs to be right length.  ");
        }
        else if ( nDigits == 14) {
            Input = true;
            printf(" Needs to be right length. 14 ");
        }

        // check card number ranges //
        if ( Input == false) {
            // Store the individual Digits of the CardNumber //
            for (int i = 0; i <= nDigits - 1; i++) {
                DigitArray[i] = DigitCardNumber % 10;
                CheckSumArray[i] = DigitArray[i];
                DigitCardNumber = DigitCardNumber / 10;

                // Prepare CheckSum //
                if ( (i % 2) == 0 ) {
                    CheckSumEven = CheckSumEven + DigitArray[i];
                }
                else {
                    CheckSumOdd = CheckSumOdd + ((2 * DigitArray[i]) % 10) + (((2 * DigitArray[i]) / 10) % 10);
                }
            }

            // Do CheckSum //
            if (((CheckSumOdd + CheckSumEven) % 10) != 0) {
                Input = true;
                printf(" Checksum Not Working.  ");
            }
           
            
        }

        //  check card starts with a valid number (AMEX = 3, VISA = 4, MC = 5)
        if ( DigitArray[nDigits-1] < 3 || DigitArray[nDigits-1] > 5) {
            Input = true;
            printf(" Needs to start with right number.  ");
        }

        if ( Input == false) {
            // What make is this card? //
            
            // Amex Check  //
            if (DigitArray[nDigits-1] == 3) {
                CardMake = "AMEX";
                if (nDigits != 15) {
                    Input = true;
                }
                if (!(DigitArray[nDigits-2] == 4 || DigitArray[nDigits-2] == 7)) {
                    Input = true;
                }
            }
            
            // Visa Check  //
            if ( DigitArray[nDigits-1] == 4) {
                CardMake = "VISA";
                if (!(nDigits == 13 || nDigits == 16)) {
                    Input = true;
                }
            }

            // Mastercard Check  //
            if (DigitArray[nDigits-1] == 5) {
                CardMake = "MASTERCARD";
                if (nDigits != 16) {
                    Input = true;
                }
                if (DigitArray[nDigits-2] < 1 || DigitArray[nDigits-2] > 5) {
                    Input = true;
                }
            }

        }



        
    } while (Input);



    printf("%s\n", CardMake);
    
// THE END //    

//    CardNumberstring = CardNumber;
//    printf(" card number string is %s\n", CardNumberstring);
//                printf(" Digit N....  %i", DigitArray[i]);                
//                printf(" Individual Digits....  %i\n", DigitArray[i]);
//                printf(" CheckSumOdd....  %i, .. CheckSumEven %i\n", CheckSumOdd, CheckSumEven);
//    printf(" card number is %lli\n", CardNumber);
//    printf(" Digit card number is %lli\n", DigitCardNumber);
//    printf(" number of Digits is  %i\n", nDigits);
//    printf(" Max card number is %i\n", maxDigit);
//    printf(" Min card number is %i\n", minDigit);

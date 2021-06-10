#include <iostream>
#include <math.h>
using namespace std; 

int main(){
    int num[100000]; // Using 100,000 instead of 1,000,000, as 1,000,000 wouldn't let me cout to console
    for(int i = 0; i < 100000; i++) {num[i] = 1;} // Initialize all values to 1
    num[0] = num[1] = 0; // Except for 0 and 1

    int p; // Holds a prime number

    for(p = 2; p < sqrt(100000); p++){
        for (int i = 2; p * i < 100000; i++){
            num[p * i] = 0;
        }
    }

    for(int k = 2; k < 100000; k++){
        if(num[k] == 1) {cout << k << " ";} // Print out primes
    }

    cout << endl;
    
    return(0);
}

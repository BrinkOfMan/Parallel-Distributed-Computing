#include<iostream> 
#include"Mvector.h"
#include<math.h> 

using namespace std; 

class Poly: public Mvector {
    int len;    /* length of the array */
    const int default_len = 10; 

    public:


    /* float operator */
    float operator()(float x) {
        float result = 0;  
        float * newArr = this->getArr();
        newArr[0] = 5; 
        for (int i = 0; i < this->getLen(); i++) {
            result = newArr[i]*pow(x, i); 
        }
        return result; 
    }
};
#include "Mvector.h"

int main() {
    Mvector V;
    Mvector W; 
    Mvector X(5);
    W = V;
    //X = W+V;
    
    V.dump();
    W.dump();
    X.dump();
}
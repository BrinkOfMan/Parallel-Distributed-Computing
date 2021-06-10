#include <iostream>
#include <queue>

using namespace std; 

/* the Register class inherits from the queue container from the STL*/
class Register : private std::queue<int>{
    protected:
        int remaining;
        const int transactionTime = 10;
    
    public:
        /* the constructor */
        Register() {
            // this refers to the current instance of the Register Class 
            this->remaining = remaining;  
        }

        /* public members */ 
        int getRemaining();
        int getTransactionTime();
        void advanceTime (int t);
        void push (const int val);
};


void Register::advanceTime(int t) {
    // t is the number of units of time to advance the register 
    while (t > 0) {
        if (remaining > t) {
            remaining -= t; 
            cout << "Progress on this cart" << endl;  
        } 
        /* else {
            t -= t; 
            cout << "This cart has been completed" << endl; 
            // remove cart from the queue

            if (there's a cart in the queue) {
                remaining = transactionTime +  <number of items in the next cart>;

                // a cart is an element in the queue. Where is this register? The register has queue properties since it's inheriting from it
            } 
        }

        if (queue is empty) {
            cout << "There are no more carts remaining in the queue" << endl; 
        }
        */
    }
}	


void Register::push	(const int val) {
    // val represents a new queue
    remaining += val; 
}


int Register::getRemaining() {
    return remaining;
}


int Register::getTransactionTime() {
    return transactionTime; 
}

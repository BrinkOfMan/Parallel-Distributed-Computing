/** @file
    @brief Simulation of a store checkout area.
    @par Strategy
    A CheckoutArea object contains multiple Register object, each 
    representing one cash register.  Carts with items arrive at each 
    register, and the time to process a cart is based on the number of
    items to be scanned in that cart plus a transaction time 
    (for a customer to pay, next customer to move up, etc.  Using 
    <a href="http://www.cplusplus.com/reference/queue/queue/">STL 
    <code>queue</code></a> class.
    @author RAB @date 10/2016 */

/* RAB notes:  
   no need for copy constr, etc, if no dynamic alloc */

#include <iostream>
#include <cassert>
#include <cstdlib>
#include "Register.h"
using namespace std;

static const int DFLT_REGISTERS = 10;

/** Represents a checkout area with multiple registers. */
class CheckoutArea {
protected:
  Register *reg; /**< Represents all register objects in the simulation */
  int size;  /**< Count of registers available in this simulation */
  
public:
  /** default and one-argument constructor */
  CheckoutArea(int sz = DFLT_REGISTERS) {
    size = sz;
    reg = new Register[size];
  }

  /** copy constructor */
  CheckoutArea(const CheckoutArea &ca) {
    size = ca.size;
    reg = new Register[size];
    copy(ca);
  };
  ~CheckoutArea() {
    delete [] reg;
    reg = 0;
  }

  /** assignment operator */
  CheckoutArea& operator=(const CheckoutArea& ca) {
    delete [] reg;
    size = ca.size;
    copy(ca);
    return *this;
  }

  /** Index operator
      @param indx Index of Register object to return
      @return Register object whose index is indx
      @pre 0 <= indx <= size */
  Register &operator[](int indx) {
    assert(0 <= indx && indx < size);
    return reg[indx];
  };
  /** Advance the simulation of each Register in reg[] by a specified 
      amount of time
      @param t non-negative number of units of time to advance the Registers */
  void advanceTime(int t) {
    for (int r = 0;  r < size;  r++) {
      cout << "Register " << r << ":  ";
      reg[r].advanceTime(t);
      cout << endl;
    }
  }

  int getSize() { return size;}

private:
  /** Helper method
      @param ca CheckoutArea object to be copied to this CheckoutArea */
  void copy(const CheckoutArea& ca) {
    for (int r = 0;  r < size;  r++)
      reg[r] = ca.reg[r];
  }
  
};

int main(int argc, char **argv) {
  int registerCount = 5;
  CheckoutArea ca(registerCount);
  int maxItemsPerCart = 50;
  int maxCartsPerIter = 10;
  int itemsPerIter = 50;

  int iter = 1;  /* count of iterations */
  while (1) {
    cout << "===== Begin iteration " << iter << " =====" << endl;

    /* invar:  all carts generated so far have been assigned to Registers */
    for (int c = 0; c < rand()%maxCartsPerIter;  c++) {
      /* add a new cart of random size to a randomly selected Register */
      ca[rand()%registerCount].push(rand()%maxItemsPerCart);
    }

    /* assert:  all carts generated for this iteration have been assigned
       to Registers */
    ca.advanceTime(itemsPerIter);

    iter++;
  }
}

#include <iostream>
using namespace std;
#include <cmath>
#include <unistd.h>
#include <sstream>
#include "mpi.h"

class Trap {

protected:
  double f(double x) {
    return pow(x, 8) - 4*pow(x,7) - pow(x,6) + 12*(x,5) + 3*pow(x,4) 
               - 4*pow(x,3) - 7*pow(x,2) - 20*x - 12;
  }

public:
  double approxIntegral(double a, double b, int n, double h) {
    double integral;
   
    integral = (f(a) + f(b))/2.0;
    for(int i = 1; i < n; i++) {
      integral = integral + f(a + i*h);
    }
    integral = integral * h;

    return integral;
  }
};

/* Begin C++ Program */
int main(int argc, char** argv) {
  /* Variables */
  int my_rank;  /** MPI id for this process */
  int nprocs;    /** Number of MPI processes in this job */
  MPI_Status   status;  /** to receive status value from certain MPI calls */

  double a = 0.0;  // lower limit for integral
  double b = 1.0;  // upper limit for integral
  int n = 1024;  // number of subdivisions

  int head = 0; // MPI rank of the head node
  int tag = 0; // programmer-defined message type (we need only one)
  
  /* Initialize for MPI communication */   
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  
  /* Compute this node's part of the integral */
  double h = (b - a) / n;
  double local_n = n / nprocs;
  double local_a = a + my_rank  * local_n*h;
  double local_b = local_a + local_n*h;
  Trap trap; /** To be used via instance method for approximation **/
  double integral = trap.approxIntegral(local_a, local_b, local_n, h);

  #define MAXHOSTNAME 25
  char hostname[MAXHOSTNAME];
  gethostname(hostname, MAXHOSTNAME);
  std::stringstream ss;
  ss << "Rank " << my_rank << " (" << hostname << ") integral=" << integral
     << endl;
  cout << ss.str();

  /* assemble all the partial results */
  double total;
  if(my_rank == head) {
    /* The head node adds up the results from all nodes */
    total = integral;  // result from the head node
    for(int source = 1; source < nprocs; source++) {
      MPI_Recv(&integral, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
      total = total + integral;
    }
  } else {
    /* Every other node sends its answer to the head */
    MPI_Send(&integral, 1, MPI_DOUBLE, head, tag, MPI_COMM_WORLD);
  }

  /* Print our answer */
  if(my_rank == head) {
    cout << "With n = " << n << " trapezoids, our estimate" << endl;
    cout << "of the integral from " << a << " to " << b << " = " << 
      total << endl;
  }
  
  /* Shut down MPI communication */
  MPI_Finalize();
}

#include <iostream>
using namespace std;
#include <cmath>
#include <unistd.h>
#include <sstream>
#include "mpi.h"

/* Begin C++ Program */
int main(int argc, char** argv) {
  /* Variables */
  int my_rank;  /** MPI id for this process, will be used as the seed for srand() */
  int nprocs;    /** Number of MPI processes in this job */
  MPI_Status   status;  /** to receive status value from certain MPI calls */


  int head = 0; // MPI rank of the head node
  int tag = 0; // programmer-defined message type (we need only one)
  
  /* Initialize for MPI communication */   
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  
  /* Compute this node's random number */
  srand(my_rank);
  double rand_value = rand()%100;

  #define MAXHOSTNAME 25
  char hostname[MAXHOSTNAME];
  gethostname(hostname, MAXHOSTNAME);
  std::stringstream ss;
  ss << "Rank " << my_rank << " (" << hostname << ") random value = " << rand_value
     << endl;
  cout << ss.str();

  /* assemble all the random numbers */
  double maximum;
  MPI_Reduce(&rand_value, &maximum, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

  /* Print our answer */
  if(my_rank == head) {
    cout << "The maximum return from rand() is " << maximum << endl;
  }
  
  /* Shut down MPI communication */
  MPI_Finalize();
}
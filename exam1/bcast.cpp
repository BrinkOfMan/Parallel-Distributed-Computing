#include <iostream>
#include <unistd.h>
using namespace std;
#include "mpi.h"

int main(int argc, char** argv) {
  int answer = 0;
  int numProcs = 0, myRank = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  srand(myRank);  /* seed random number generator */
  sleep(rand()%5);

  if (myRank == 0) {
    answer = 42;
  }
  
  cout << "BEFORE the broadcast, process " << myRank
       << "'s answer = " << answer << endl; 
  
  MPI_Bcast(&answer, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
  cout << "AFTER the broadcast, process " << myRank
       << "'s answer = " << answer << endl; 
  
  MPI_Finalize();
  
  return 0;
}

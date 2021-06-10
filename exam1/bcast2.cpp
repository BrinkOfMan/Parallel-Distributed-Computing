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
  MPI_Status   status;
  srand(myRank);  /* seed random number generator */
  sleep(rand()%5);

  
  if(myRank == 0) {
    cout << "BEFORE the send, process " << myRank
       << "'s answer = " << answer << endl;
    answer = 42;
    for(int source = 1; source < numProcs; source++) {
      MPI_Send(&answer, 1, MPI_INT, source, 0, MPI_COMM_WORLD);
    }
    cout << "AFTER the send and recv, process " << myRank
       << "'s answer = " << answer << endl;
  } else {
    cout << "BEFORE the send, process " << myRank
       << "'s answer = " << answer << endl;
    MPI_Recv(&answer, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
    cout << "AFTER the recv and recv, process " << myRank
       << "'s answer = " << answer << endl;
  }
  
  MPI_Finalize();
  
  return 0;
}

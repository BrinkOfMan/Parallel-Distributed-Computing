#include <iostream>
#include <unistd.h>
#include "Sync.h"
#include "mpi.h"
using namespace std;

int main(int argc, char** argv){
  
  /* Variables */
  int my_rank;  /** MPI id for this process, will be used as the seed for srand() */
  int nprocs;    /** Number of MPI processes in this job */
  int head = 0; // MPI rank of the head node
  MPI_Status   status;  /* to receive status value from certain MPI calls */

  /* Initialize for MPI communication */   
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);


  if(my_rank == head) { // Head node
    SyncServer::run();
    
  }
  else { // Worker nodes
    Sync proc(head);
    proc.send(PRINT, "hello");
    char* rec = proc.recv(ACK);
    cout << rec << endl;
    proc.send(QUIT, "");
    rec = proc.recv(ACK);
    cout << rec << endl;
  }
  /* Shut down MPI communication */
  MPI_Finalize();
  return 0;
}
  
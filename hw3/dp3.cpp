#include <iostream>
#include <unistd.h>
#include "dp.h"
#include "mpi.h"
using namespace std;

int main(int argc, char** argv){
  
  /* Variables */
  int my_rank;  /** MPI id for this process, will be used as the seed for srand() */
  int nprocs;    /** Number of MPI processes in this job */
  MPI_Status   status;  /* to receive status value from certain MPI calls */

  /* Initialize for MPI communication */   
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  int head = nprocs - 1; // MPI rank of the head node


  if(my_rank == head) { // Head node
    SyncServer::run();
  }
  else { // Worker nodes
    Sync proc(head);

    /* Going to comment out things I don't use

    // PRINT msg
    proc.send(PRINT, "hello");
    rec = proc.recv(ACK);
    cout << rec << endl;

    // ECHO msg
    proc.send(ECHO, "hello");
    rec = proc.recv(ACK);
    cout << "ECHO message returned: " << rec << endl;

    */

    // GETCHOP msg, LEFT
    proc.send(GETCHOP, "L");
    char* rec = proc.recv(ACK);
    cout << "WORKER: Philosopher " << my_rank << " called GETCHOP/L responded with " << rec << endl << endl;
    sleep(2);

    // GETCHOP msg, RIGHT
    proc.send(GETCHOP, "R");
    rec = proc.recv(ACK);
    cout << "WORKER: Philosopher " << my_rank << " called GETCHOP/R responded with " << rec << endl << endl;
    sleep(2);

    // PUTCHOP msg, LEFT
    proc.send(PUTCHOP, "L");
    rec = proc.recv(ACK);
    cout << "WORKER: Philosopher " << my_rank << " called PUTCHOP/L responded with " << rec << endl << endl;

    sleep(2);

    // QUIT msg
    proc.send(QUIT, "");
    rec = proc.recv(ACK);
    cout << rec << endl;
  }
  /* Shut down MPI communication */
  MPI_Finalize();
  return 0;
}
  
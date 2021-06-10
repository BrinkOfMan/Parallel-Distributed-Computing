#include <iostream>
#include <unistd.h>
#include <sys/select.h>
#include <string>
#include "deadlock.h"
#include "mpi.h"
using namespace std;

/* pause execution for msec thousandths of a second, on Linux */
void stosleep(long msec) {
  struct timeval tv;
  tv.tv_sec = msec/1000;
  tv.tv_usec = 1000 * msec % 1000000;
  select(0, NULL, NULL, NULL, &tv);
}


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

  srand(my_rank); // Seed the random number generator to the processes' rank

  int retry; // Just declaring this here for some future shenanigans


  if(my_rank == head) { // Head node
    SyncServer::run();
  }
  else { // Worker nodes

    Sync proc(head); // Synchronize everything
    char* rec;

    for(int i = 0; i < 10; i++){ // Repeat "forever"

      /* Going to comment out things I don't use and put them here

      // PRINT msg
      proc.send(PRINT, "hello");
      rec = proc.recv(ACK);
      cout << rec << endl;

      // ECHO msg
      proc.send(ECHO, "hello");
      rec = proc.recv(ACK);
      cout << "ECHO message returned: " << rec << endl;

      */

      cout << "P" << my_rank << ": thinking\n";
      // stosleep(rand()%2000); // Think

      retry = 1;
      while(retry){
        // GETCHOP msg, LEFT
        proc.send(GETCHOP_BLOCKING, "L");
        rec = proc.recv(ACK);
        if (string(rec).compare("") == 0) {cout << "P" << my_rank << ": now possesses " << my_rank << endl; retry = 0;}
        else {cout << "P" << my_rank << ": retrying for " << my_rank << endl;}
        stosleep(rand()%1000); // Baby delay
      }
      
      stosleep(rand()%2000); // Delay between picking up left + right chopsticks to cause a deadlock

      retry = 1;
      while(retry){
        // GETCHOP msg, RIGHT
        proc.send(GETCHOP_BLOCKING, "R");
        rec = proc.recv(ACK);
        if (string(rec).compare("") == 0) {cout << "P" << my_rank << ": now possesses " << ((my_rank+1) % nprocs-1) << endl; retry = 0;}
        else {cout << "P" << my_rank << ": retrying for " << ((my_rank+1) % nprocs-1) << endl;}
        stosleep(rand()%1000); // Baby delay
      }

      cout << "P" << my_rank << ": eating\n";
      stosleep(rand()%2000); // Eat

      // PUTCHOP msg, LEFT
      proc.send(PUTCHOP, "L");
      rec = proc.recv(ACK);
      if (strlen(rec) == 0) {cout << "P" << my_rank << ": returning " << my_rank << endl;}
      else {cout << "P" << my_rank << ": unable to return " << my_rank << endl;}
      stosleep(rand()%250); // Longer baby delay

      // PUTCHOP msg, RIGHT
      proc.send(PUTCHOP, "R");
      rec = proc.recv(ACK);
      if (strlen(rec) == 0) {cout << "P" << my_rank << ": returning " << ((my_rank+1) % nprocs-1) << endl;}
      else {cout << "P" << my_rank << ": unable to return " << ((my_rank+1) % nprocs-1) << endl;}
      stosleep(rand()%250); // Longer baby delay

    }
    // QUIT msg
    proc.send(QUIT, "");
    rec = proc.recv(ACK);
    cout << rec << endl;

    delete rec; // De-allocate rec
  }
  /* Shut down MPI communication */
  MPI_Finalize();
  return 0;
}
  
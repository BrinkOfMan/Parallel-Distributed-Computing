#include <iostream>
#include <unistd.h>
#include <sys/select.h>
#include <string>
#include "starvation.h"
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

  bool retry, eaten; // Declaring these here for loops


  if(my_rank == head) { // Head node
    SyncServer::run();
  }
  else { // Worker nodes

    Sync proc(head); // Synchronize everything
    char* rec;

    stosleep(rand()%5000);

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
      eaten = 0;
      while(!eaten){
      
        retry = 1;
        while(retry){
          // GETCHOP msg, LEFT
          proc.send(GETCHOP, "L");
          rec = proc.recv(ACK);
          char msg[] = "";
          if (string(rec).compare(msg) == 0) {
            cout << "P" << my_rank << ": now possesses " << my_rank << endl; retry = 0;
          }
          else { // Retry
            cout << "P" << my_rank << ": retrying for " << my_rank << endl;
          }
          stosleep(rand()%1000);
        }
        
        stosleep(rand()%3000); // Delay between picking up left + right chopsticks to cause a deadlock


        // GETCHOP msg, RIGHT
        proc.send(GETCHOP, "R");
        rec = proc.recv(ACK);

        stosleep(rand()%1000); // Baby delay
        char msg[] = "";
        if (string(rec).compare(msg) == 0) {
          cout << "P" << my_rank << ": now possesses " << ((my_rank+1) % nprocs-1) << endl; retry = 0;
          eaten = 1;
        }
        else { // Put down LEFT chopstick after failing to grab right chopstick
          // PUTCHOP msg, LEFT
          proc.send(PUTCHOP, "L");
          rec = proc.recv(ACK);
          if (string(rec).compare(msg) == 0) {cout << "P" << my_rank << ": returning " << my_rank << endl;}
          else {cout << "P" << my_rank << ": unable to return " << my_rank << endl;}
          cout << "P" << my_rank << ": retrying for both chopsticks" << endl;
        }
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
  
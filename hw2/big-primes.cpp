#include <iostream>
using namespace std;
#include <cmath>
#include <unistd.h>
#include <sstream>
#include "mpi.h"
#include <fstream>

int main(int argc, char** argv){
  /* Variables */
  int my_rank;  /** MPI id for this process, will be used as the seed for srand() */
  int nprocs;    /** Number of MPI processes in this job */
  int found = 0; // 0 means no prime has been found. Keep going!
  MPI_Status   status;  /** to receive status value from certain MPI calls */


  int head = 0; // MPI rank of the head node
  int tag = 0; // programmer-defined message type (we need only one)

    int array_size = 78498;
  long arr[array_size];
  
  /* Initialize for MPI communication */   
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  
  int iteration = 1;

  ifstream infile("primes.list");

  // Fill the array with prime numbers
  for(int i = 0; i < array_size; i++){
    infile >> arr[i];
  }

  // ############## Start the loop to find primes ##############
  while(found != 1){

    /* Compute / fabricate this node's random 2*k digit number */
    // Note: For my tests, 2*k would equal 12
    srand(my_rank + 8 * iteration); // Not adding a way to change the seed gives repeated values. This should change it up.
    const long long M = 10000;  // 10^4
    long long bigrand = (rand() % M);
    bigrand += (rand()%M)*M;
    bigrand += (rand()%M)*M*M;
    // bigrand is between 0 and M^3


    cout << "rand value: " << bigrand << endl;

    // Test if bigrand is prime
    bool is_prime = true;
    int factor_index; // If bigrand is factorable.

    for(int i = 0; i < array_size && arr[i] < sqrt(bigrand); i++){
      if(bigrand % arr[i] == 0) { is_prime = false; factor_index = i; break;}
    }

    if(is_prime){
      cout << "\n\n\n\n" << bigrand << " IS A PRIME NUMBER. STOP ALL PROCESSES FROM CONTINUING TO SEARCH FOR PRIMES.\n\n\n\n";
      found = 1;  
    }
    else { cout << bigrand << " Is not prime. It is divisible by " << arr[factor_index] << endl; }

    // Point-to-point communication time

    if(my_rank == head) {
      // Nothing has been found yet
      if (found == 0){
        for(int source = 1; source < nprocs; source++) {
          MPI_Send (&found, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
        }
        for(int source = 1; source < nprocs; source++) {
          MPI_Recv(&found, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        }
      }
      // Something has been found!
      if (found == 1){
        for(int source = 1; source < nprocs; source++) {
          MPI_Send (&found, 1, MPI_INT, source, tag, MPI_COMM_WORLD);
        }
      }
    } else {
        /* Every other node recv() before they send() their answer to the head */
        MPI_Recv(&found, 1, MPI_INT, head, tag, MPI_COMM_WORLD, &status);
        MPI_Send(&found, 1, MPI_INT, head, tag, MPI_COMM_WORLD);
    }

    iteration++;
  }
  

  // /* Shut down MPI communication */
  MPI_Finalize();

  return 0;

}
#include <iostream>
#include <stdlib.h>
using namespace std;
#include <cmath>
#include <unistd.h>
#include <sstream>
#include "mpi.h"
#include <fstream>

int array_size = 50847535;
int arr[50847535];

int main(int argc, char** argv){

  /* Variables */
  int my_rank;  /** MPI id for this process, will be used as the seed for srand() */
  int nprocs;    /** Number of MPI processes in this job */
  int found = 0; // 0 means no prime has been found. Keep going!
  MPI_Status   status;  /** to receive status value from certain MPI calls */
  
  
  // Tests for if bigrand is prime
  bool is_prime = true;
  int factor_index; // If bigrand is factorable.


  int head = 0; // MPI rank of the head node
  int tag = 0; // programmer-defined message type (we need only one)
  
  /* Initialize for MPI communication */   
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  srand(my_rank + 1); // Seed a random number 

  int news[nprocs]; // result from an individual process to see if a prime has been found/

  ifstream infile("primes.list");

  // Fill the array with prime numbers
  for(int i = 0; i < array_size; i++){
    infile >> arr[i];
  }

  // ############## Start the loop to find primes ##############
  while(found != 1){

    /* Compute / fabricate this node's random 2*k digit number */
    const long long M = 1000000;  // 10^6
    long long bigrand = (rand() % M);
    bigrand += (rand()%M)*M;
    bigrand += (rand()%M)*M*M;
    // bigrand is between 0 and M^3

    is_prime = true;

    cout << "rand value: " << bigrand << endl;

    for(int i = 0; i < array_size && arr[i] < sqrt(bigrand); i++){
      if(bigrand % arr[i] == 0) { is_prime = false; factor_index = i; break;}   
    }

    if(is_prime){
      cout << "\n\n\n\n" << bigrand << " IS A PRIME NUMBER. STOP ALL PROCESSES FROM CONTINUING TO SEARCH FOR PRIMES.\n\n\n\n";
      found = 1;  
    }
    else { cout << bigrand << " Is not prime. It is divisible by " << arr[factor_index] << endl; }

    // Collective communication time

    MPI_Gather(&found, 1, MPI_INT, &news, nprocs, MPI_INT, head, MPI_COMM_WORLD);
    for(int i = 0; i < nprocs; i++){
      if(news[i] == 1){
        found = 1;
        break;
        // This sometimes catches a prime, but gets stuck and I'm not sure why.
      }
    }

    MPI_Bcast(&found, 1, MPI_INT, head, MPI_COMM_WORLD); 
  }
  

  // /* Shut down MPI communication */
  MPI_Finalize();

  return 0;

}
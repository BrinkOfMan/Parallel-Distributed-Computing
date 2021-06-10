#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <unistd.h>
#include "mpi.h"
using namespace std; 

int ipow(int base, int exp){

    int max = pow(base, exp);

    int num[max];

    for(int i = 0; i < max; i++) {num[i] = 1;} // Initialize all values to 1
    num[0] = num[1] = 0; // Except for 0 and 1

    int p; // Holds a prime number

    for(p = 2; p < sqrt(max); p++){
        for (int i = 2; p * i < max; i++){
            num[p * i] = 0;
        }
    }

    std::stringstream output;
    output << "primes.out"<< exp ;

    ofstream outfile(output.str());

    for(int k = 2; k < max; k++){
        if(num[k] == 1) {outfile << k << endl;} // Print out primes
    }

    outfile.close();

    for(int i = max; i > 0; i--){
        if(num[i] == 1) { return i; }
    }

    return 0;
}

int main(int argc, char** argv){
    /* Variables */
    int my_rank;  /** MPI id for this process */
    int nprocs;    /** Number of MPI processes in this job */
    MPI_Status   status;  /** to receive status value from certain MPI calls */
    int head = 0; // MPI rank of the head node
    int tag = 0; // programmer-defined message type (we need only one)

    /* Initialize for MPI communication */   
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    /* Print the list of primes & return the max prime for that head */
    int ret = ipow(10, my_rank + 4);

    #define MAXHOSTNAME 25
    char hostname[MAXHOSTNAME];
    gethostname(hostname, MAXHOSTNAME);
    std::stringstream ss;
    ss << "Rank " << my_rank << " (" << hostname << ") had the highest prime of " << ret
     << endl;
    cout << ss.str();

    /* Shut down MPI communication */
    MPI_Finalize();
    
    return(0);
}

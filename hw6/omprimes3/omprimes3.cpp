#include <omp.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>

using std::cout;
using std::endl;
using std::vector;

void usage(void)
{
  cout << "sieve <prime max> <thread count>" << endl;
  cout << "<prime max> is upper bound on value of primes found." << endl;
  cout << "<thread count> is the number of threads to use." << endl;
}

void parse_args(int argc, char **argv, int *prime_maxp, int*num_threadsp) {
  if (argc != 3)
    {
      cout << "Invalid number of arguments!" << endl;
      usage();
      exit(0);
    }
  
  *prime_maxp = atoi(argv[1]);
  *num_threadsp = atoi(argv[2]);
  
  if (*prime_maxp < 2)
    {
      cout << "<range> Must be an integer greater than or equal to 2.";
      usage();
      exit(0);
    }
 
    if (*num_threadsp < 1)
    {
      cout << "<thread count> Must be an integer N where 1 <= N <= ";
      usage();
      exit(0);
    }
 
    if (*num_threadsp > omp_get_max_threads())
    {
        *num_threadsp = omp_get_max_threads();
    }
 
    /*
    // Make sure we haven't created too many threads.
    int temp = (*prime_maxp - 1) / *num_threadsp;
    if ((1 + temp) < (int)sqrt((double)*prime_maxp))
    {
        cout << "Too many threads requested!" << endl;
        cout << "The first thread must have a block size >= sqrt(n)." << endl;
        exit(1);
    }
    */
 
}

int main(int argc, char ** argv)
{
  // argv[1]: requested upper-bound on primes.
  // argv[2]: requested number of threads to create.

  int prime_max;  // upper bound on prime values
  int num_threads;  // number of threads to create

  parse_args(argc, argv, &prime_max, &num_threads);
  
  int count = 0;  // Global count of primes found so far
  int i;  // loop control

  
  // Array marked[].  marked[i] assigned to 1 if i is not prime
  char * marked = (char *)malloc(prime_max + 1);
  marked[0] = 1;
  marked[1] = 1;
#pragma omp parallel for default(shared) private(i) 
  for (i = 2; i <= prime_max; ++i) 
    marked[i] = 0;

  int p = 2;  // holds an integer known to be prime
  
  while (p * p <= prime_max)
    {
      int base = p * p;  // note: lower multiples of p are already marked

#pragma omp parallel for default(shared) private(i) 
      for (i = base; i <= prime_max; i += p)
	marked[i] = 1;

#pragma omp single
      while (marked[++p])  
	;
    } 
  
#ifdef KEEP_PRIMES
  std::vector<int> pset;
#endif /* KEEP_PRIMES */
  for (i = 2; i <= prime_max; ++i)
    {
      if (marked[i] == 0) {
#ifdef KEEP_PRIMES
	pset.push_back(i);
#endif /* KEEP_PRIMES */
	count++;
      }
    }
  
  free(marked);
      

  // Print the results 
  cout << count << " primes are less than or equal to " << prime_max << endl;

#ifdef KEEP_PRIMES
  cout << "First few:  "; // (up to 20 primes)
  i = 0;
  while (i < 19 && i < count-1)
    cout << pset[i++] << ", ";
  cout << pset[i] << endl;
#endif /* KEEP_PRIMES */

  return 0;
}

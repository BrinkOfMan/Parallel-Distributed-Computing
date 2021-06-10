#include <omp.h>
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char ** argv)
{
  int val = 1;
  int p;
   
  cout << "Threads: ";
#pragma omp parallel sections default(shared) 
  {
#pragma omp section
    {
      p = omp_get_thread_num();
      val = val + 1000;
      cout << p << " ";
    }
    
#pragma omp section
    {
      p = omp_get_thread_num();
      val = val << p;
      cout << p << " ";
    }

#pragma omp section
    {
      p = omp_get_thread_num();
      val = val + p;
      cout << p << " ";
    }

  }

  cout << endl;
  cout << "At end, val = " << val << endl;
  return 0;
}
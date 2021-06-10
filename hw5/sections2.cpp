#include <omp.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char ** argv)
{
  int val = 1;
  int p;
  stringstream s;

  s << "Threads: ";
#pragma omp parallel sections \
    shared(s) private(p) reduction (+: val)
  {
#pragma omp section
    {
      p = omp_get_thread_num();
      val = val + 1000;
      #pragma omp critical
      {
        s << p;
        s << " ";
      }     
    }
    
#pragma omp section
    {
      p = omp_get_thread_num();
      val = val + p;
      #pragma omp critical
      {
        s << p; 
        s << " ";
      }     
    }

#pragma omp section
    {
      p = omp_get_thread_num();
      val = val + p;
      #pragma omp critical
      {
        s << p;
        s << " ";
      }
    }

  }

  cout << s.str() << endl;
  cout << "At end, val = " << val << endl;
  return 0;
}
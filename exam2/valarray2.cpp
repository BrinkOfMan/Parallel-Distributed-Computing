#include <iostream>
#include <string>
#include <cstdlib>
#include <tbb/concurrent_vector.h>
#include "tbb/tbb.h"
using namespace std;
using namespace tbb;

#define MAX_SEQ 7
#define NSEQS 120
#define PROTEIN "the cat in the hat wore the hat to the cat hat party"

string get_seq(int max) {
  int len = 1 + rand()%max;
  string ret(len, '?');
  for (int i=0;  i<len; i++)
    ret[i] = 'a' + rand()%26;
  return ret;
}

int val(const char *str1, const char *str2) {
  if (*str1 == '\0' || *str2 == '\0')
    return 0;
  // both argument strings non-empty
  if (*str1 == *str2)
    return 1 + val(str1 + 1, str2 + 1);
  else // first characters do not match
    return max(val(str1, str2 + 1), val(str1 + 1, str2));
}


int main() {
  concurrent_vector<string> seqs; /* vector of sequences to process*/
  concurrent_vector<string> arr[MAX_SEQ+1];
    /* array of vectors of sequences, indexed by return from val() */
  string protein(PROTEIN);

  cout << "Creating list of sequences..." << endl;
  for (int a = 0; a < NSEQS;  a++)
    seqs.push_back(get_seq(MAX_SEQ));

  cout << "Entering sequences into arr[] according to return from val()..."
       << endl;


  parallel_for(blocked_range<size_t>(0, seqs.size()),
                      [=](blocked_range<size_t> r){
                          for(size_t b = r.begin(); b != r.end(); b++){
                            int ret;  /* holds a return value from val() */
                            ret = val(seqs.at(b).c_str(), protein.c_str());
                            // arr is a concurrent_vector, which is thread-safe
                            arr[ret].push_back(seqs.at(b)); 
                            // arr is a concurrent_vector, which is thread-safe
                          }
                      });
  /*
  for (int b = 0; b < seqs.size();  b++) {
    ret = val(seqs.at(b).c_str(), protein.c_str());
    arr[ret].push_back(seqs.at(b));  
  }
  */
  cout << endl;

  cout << "Printing sequences according to return from val()..." <<endl;
  for (int c = 0;  c <= MAX_SEQ;  c++) {
    if (arr[c].size() == 0)
      continue;
    cout << c << ": ";
    for (int d = 0;  d < arr[c].size(); d++)
      cout << " " << arr[c].at(d);
    cout << endl;
  }

  return 0;
}


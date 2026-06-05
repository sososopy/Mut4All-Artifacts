
#include <stdio.h>
int gValue = 1;
class NPair /*: protected Pointers */ {
public:
  int bins[10];
  //int exclusion(int n) __attribute__((const));
  __attribute__((const)) int exclusion (int n) const {
    printf ("before %d\n", bins[0]);
    NPair* pt = const_cast<NPair*>(this);
    pt->bins[0] = 1;
    printf ("after %d\n", bins[0]);
    return gValue++; // expect a error
  }
  NPair (void) {
     bins[0] = 3;
  }
};

// extern int * __restrict bins;
class NPair nPair;

int main (void)
{
  int tmp = nPair.bins[0];
  
  (void)nPair.exclusion(1);
  
  return tmp == nPair.bins[0];
}

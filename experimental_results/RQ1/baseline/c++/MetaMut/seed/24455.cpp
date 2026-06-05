
#include <stdio.h>
#include <string.h>

int hash_it(const char* d, const int prime) __attribute__((noinline));
int hash_it(const char* d, const int prime)
{
      int n = strlen(d);
      int h = n; 
      
      for (int i = 0; i < n; i++, d++)
          h = (777*h) + (*d);
      
      return ((h >= 0) ? (h % prime) : (-h % prime)); 
}

int main ()
{
	int lret = 0;
	const char* lsdata = "ABCD";
	lret = hash_it(lsdata, 10);
	printf("%d, %s\n", lret, lsdata);

	return 0;
}


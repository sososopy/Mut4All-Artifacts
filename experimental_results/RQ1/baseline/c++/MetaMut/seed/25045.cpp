

#include <iostream>

#define ALIGN  __attribute__ ((aligned(32)))

class Foo
{
public:
  int a;

  double x[24] ALIGN;
  int b;
} ALIGN;

  
int main()
{
  for (int i=0; i<5; i++)
    {
      Foo *p;
      double *a;
      p = new Foo;
      a = new double;
      unsigned long pv;
      pv = (unsigned long) p->x;
      if (pv % 32 != 0)
	std::cout << "pv = " << pv << " is not divisble by 32" <<
	  " remainder is " << pv%32 << "\n";
      else
	std::cout << "pv = " << pv << " is divisble by 32\n";
    }
}


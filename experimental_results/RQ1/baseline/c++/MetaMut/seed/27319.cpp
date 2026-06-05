
#include <stdio.h>
int count = 0;
struct A {
  __attribute__((malloc,noinline))
  static void* operator new(unsigned long sz){++count;return ::operator new(sz);}
  static void operator delete(void* ptr){--count;::operator delete(ptr);}
};
int main(){
  delete new A;
  printf("%d\n",count); // Should print 0.
}




#include <vector>

#ifndef SIZE
#define SIZE 100000
#endif
struct S
{
  int a;
};

struct T
{
  std::vector<S> arr[SIZE]{}; // remove the default-initializer {} to compile fast
};

int main(int argc, char** argv)
{
  T t;
  for (int i=0; i<SIZE; ++i) {
    t.arr[i].push_back(S{argc * i}); // use argc so that gcc doesn't optimize everything out
  }
  return t.arr[SIZE-1][0].a;
}

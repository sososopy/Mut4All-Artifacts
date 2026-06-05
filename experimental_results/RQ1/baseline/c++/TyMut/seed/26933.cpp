
#include <cstdlib>

struct S
{
  void method(){}
};


template<size_t sz, size_t tot>
class genpool
{
};

template <class T>
class mempool
{
private:
  genpool<sizeof(T), 10*sizeof(T)> p;
};


template <class obj, class mthd>
class functor
{
private:
  obj& o;
  mthd m;
  static mempool<functor<obj, mthd> > pool;
};

template <class obj, class mthd>
mempool<functor<obj, mthd> > functor<obj, mthd>::pool;

int main()
{
  typedef void (S::*m)();
  typedef functor<S, m> fctr;

  fctr f;
}



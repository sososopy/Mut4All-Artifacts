
struct A
{ 
  A operator+ (A a)
  { 
    return a;
  }
};

template < class T >
void g (T t)
{ 
  auto x = t + x;
}

int
main ()
{ 
  g (A ());
  return 0;
}

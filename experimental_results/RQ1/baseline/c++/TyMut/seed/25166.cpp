

struct C
{
  template<typename... Ts>
  int operator()(Ts &&...)
  {
    return sizeof...(Ts);
  }
};

int
main ()
{
  C {} (1, 1L, 1LL, 1.0);
  char a;		// { dg-warning "unused variable" }
  short b;		// { dg-warning "unused variable" }
  int c;		// { dg-warning "unused variable" }
  long d;		// { dg-warning "unused variable" }
  long long e;		// { dg-warning "unused variable" }
  float f;		// { dg-warning "unused variable" }
  double g;		// { dg-warning "unused variable" }
}


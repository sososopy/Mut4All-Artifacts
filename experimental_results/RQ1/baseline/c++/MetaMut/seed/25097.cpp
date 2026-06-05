

struct S {};

template < typename T > struct A
{ 
  template < S > void f () {}

  static void * g ()
  { 
    return (void *) f < a >;
  }

  static S a;
};

void * f ()
{ 
  return A < int >::g ();
}

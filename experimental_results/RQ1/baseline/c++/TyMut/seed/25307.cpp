

template < typename T >
struct A : T
{ 
  void foo () { this->A::template bar <>; }
};

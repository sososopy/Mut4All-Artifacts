
template < typename T > struct A
{ 
  A ();
  ~A ();
  T t;
};

void f (A < int > a)
{ 
  for (auto x : (A < int >[]) { a })
    ;
}

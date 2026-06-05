

struct S; //OK: struct S {  void operator () (S); }; 

template < typename > auto f (S s)->decltype (s (s));

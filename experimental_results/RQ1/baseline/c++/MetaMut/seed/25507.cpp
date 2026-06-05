template<typename T>
union point 
{ 
  T vec __attribute__((__vector_size__((sizeof(T)<<1))));
 struct { T x,y; }; 
 };


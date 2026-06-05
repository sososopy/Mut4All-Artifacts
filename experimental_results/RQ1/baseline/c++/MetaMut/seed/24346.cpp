
template<typename C, typename T> struct A<T (C::*)() const>
{
  int ok;
};


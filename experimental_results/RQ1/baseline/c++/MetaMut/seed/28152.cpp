
template <typename T> class X;

template<>
class X<void>
{
  template<typename U> void f (U);
};

template<>
template<typename U>
void X<void>::f (U) 
{
}


struct A { 
  int a; 

  A() {}

  A(int i)
    :a(i)
  { }
};

template<typename T>
class Box: public T
{
public:
  Box<T>* clone() const {return new Box<T>(*this);}

  Box<T>& operator=(const Box<T>& t)
  {
    T::operator=(t);
    return *this;
  }

  Box<T>& operator=(const T& t)
  {
    T::operator=(t);
    return *this;
  }

  using T::T;

  Box() = default;
  Box(const Box<T>&) = default;
  explicit Box(const T& t):T(t) {}
};

int main()
{
  Box<A> a;
  return 0;
}

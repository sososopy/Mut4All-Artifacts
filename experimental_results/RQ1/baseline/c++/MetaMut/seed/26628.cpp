

template <typename T>
struct A
{
  struct N { };
};

template <typename T>
class B : public A<T>
{
  using parent_t = A<T>; 

  public:
  struct N : parent_t::N { };
};

template <typename T>
void f()
{
  typename B<T>::N n; // comment out for correct error
  typename B<T>::parent_t a;
}

int main()
{
  f<int>();
}


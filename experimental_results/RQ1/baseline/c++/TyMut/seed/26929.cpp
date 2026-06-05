

template <class T> concept bool A
  = requires { typename T::Type; } && T::Type::value;

template <class T> requires !A<T>
void f() { }

struct B { };

int main()
{
  f<B>();
}


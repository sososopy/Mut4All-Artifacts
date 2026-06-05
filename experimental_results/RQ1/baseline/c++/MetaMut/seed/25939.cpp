
struct A {};

template<typename> struct B
{
  A a[1] = {};
};

B<int> b;


class A
{
};

template<typename T>
class B
{
public:
  A v[1] = {};
};

B<int> b;

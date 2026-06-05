template<int N>
struct foo
{
  static const unsigned value = 8;
};

template<unsigned N>
struct aligned_union
{
  alignas(foo<N>::value) char data[N];
};

int main()
{
  aligned_union<8> a;
}

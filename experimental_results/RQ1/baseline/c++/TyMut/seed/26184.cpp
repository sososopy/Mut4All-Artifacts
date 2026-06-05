
struct A
{
  static constexpr int&& i = 0;
};

int j = A::i;

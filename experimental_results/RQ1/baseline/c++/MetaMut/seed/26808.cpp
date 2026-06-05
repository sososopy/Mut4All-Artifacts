
struct G {};
struct L
{
  enum class T { S } t;
  G f() const
  {
    switch (t)
      {
      case T::S: return G();
      }
  }
};

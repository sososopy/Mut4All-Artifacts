
struct A
{
  static const int i;
};

const int A::i = (struct { int j=0; }){}.j;

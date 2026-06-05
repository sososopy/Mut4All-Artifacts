
struct S
{
  int &&a;
  int b[1] {a};
};
S c[] {{2}};



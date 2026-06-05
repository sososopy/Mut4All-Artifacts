
// cat ice.cpp
union U
{
  struct
  {
    unsigned char a;
  };

  unsigned char b[1];
};

void f(unsigned char a)
{
  union U u = { .a = a };
}


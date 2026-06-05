

struct S
{
  int a, b, c;
};

extern void callee(const S &s);

void test()
{
  const S s{1,2,3};
  callee(s);

  callee((const S){1,2,3});
}


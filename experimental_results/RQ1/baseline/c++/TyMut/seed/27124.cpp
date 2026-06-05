
constexpr int lt_fun(const char& c1, const char& c2) {
  return c1 < c2;
}

constexpr bool doit()
{
  char *pc = new char;
  const char* s = "a";

  *pc = 'b';
            lt_fun(*s, *pc); // a < b

  *pc = 'a';
  bool lt = lt_fun(*s, *pc); // a < a

  delete pc;

  return !lt;
}

int main(int argc, char *argv[])
{
  static_assert(doit());
  return doit() ? 0 : -1;
}

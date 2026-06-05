

enum One {
  One_Magic
};

enum Two {
  Two_Magic
};

void test()
{
  static_assert( One_Magic == Two_Magic, "");
}


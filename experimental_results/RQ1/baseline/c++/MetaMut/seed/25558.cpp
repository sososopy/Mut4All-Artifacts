
#include <string>

static int main_worker(int argc)
{
  std::string s[32];
  if (argc < 2)
    throw 42;
  return argc;
}

int main(int argc, char **argv)
{
  try {
    return main_worker(argc);
  }
  catch (int i) {
    return i;
  }
}

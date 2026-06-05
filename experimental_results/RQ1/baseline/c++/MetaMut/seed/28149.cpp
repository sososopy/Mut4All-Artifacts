
int main()
{
  struct thing
  {
    char str[100] = "foo";
    //char str[100] = {'f', 'o', 'o', '\0'}; // this is fine though
  };

  thing foo;
  foo = {}; // clang will compile this, but gcc won't
}


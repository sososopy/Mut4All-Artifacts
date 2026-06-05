
  int main()
  {
    int &&x = 0;
    decltype(auto) y = x; // Should behave like  int &&y = x;  and thus give a
                          // compilation error since  x  is an lvalue.
  }


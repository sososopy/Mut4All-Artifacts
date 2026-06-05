
template<typename CharT, CharT... str>
  void
  operator""_foo
  {
    ChatT arr[]{str...};
  }

U"\x10000\x10001\x10002"_foo;


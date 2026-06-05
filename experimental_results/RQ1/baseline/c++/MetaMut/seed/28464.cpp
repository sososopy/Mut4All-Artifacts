
    struct string
    {
      char str[8] = "       ";
      const char* data() { return str; }
    };

    template<bool a, bool b>
    consteval string test()
    {
      string str { };
      char* p = str.str;

      auto append = [&p](const char* s)
      {
        while (*s != '\0') *p++ = *s++;
        ++p;
      };

      if (a) append("abc");
      if (b) append("xyz");
      return str;
    }

    auto f() { return test<true,true>(); }


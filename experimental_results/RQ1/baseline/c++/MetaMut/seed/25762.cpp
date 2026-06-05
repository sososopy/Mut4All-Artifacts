
  extern "C"
  {
    __attribute__((__used__))
    static void
    foo(void)
    {
    }
  }

  asm("call foo");


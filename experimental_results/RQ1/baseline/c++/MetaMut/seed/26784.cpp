

  template<typename T>
  inline void SetRdx(T value)
  {
      register auto __foo asm ("rdx") = value;
      asm volatile ( "" :: "r" (__foo) );
  }


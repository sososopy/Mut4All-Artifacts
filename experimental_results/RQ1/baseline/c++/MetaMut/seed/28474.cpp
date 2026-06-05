
extern "C" int puts(const char*);

struct A
{
  constexpr A()  { }
  ~A() { puts("bye"); }
};

namespace
{
  struct constant_init
  {
    union {
      A obj;
    };
    constexpr constant_init() : obj() { }

    ~constant_init() { /* do nothing, union member is not destroyed */ }
  };


  // Single-threaded fallback buffer.
  constinit constant_init global;
}

extern "C" A* get() { return &global.obj; }


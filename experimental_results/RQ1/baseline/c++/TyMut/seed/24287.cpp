
extern int f(int *);

static int i __attribute__ ((__weakref__("f")));

template<typename T>
class X
{
  static __thread T* value_;
};

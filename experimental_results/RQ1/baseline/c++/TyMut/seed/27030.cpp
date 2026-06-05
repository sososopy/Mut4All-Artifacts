
#include <string>

namespace N
{
  struct S { std::string str; };
  // extern declaration in a header
  extern thread_local S * ptd;

  // accessing the extern declared ptd here
  inline
  S * get_ptd ()
  {
    if (! ptd)
      ptd = new S;
    return ptd;
  }
} // namespace N


#include "def.hxx"

namespace N
{
  // definition of ptd
  thread_local S * ptd = nullptr;
} // namespace N

#include "def.hxx"

namespace N
{
  __declspec(dllexport)
  void * foo ()
  {
    // invoking inline get_ptd() function to get the value in ptd
    return get_ptd ();
  }
}

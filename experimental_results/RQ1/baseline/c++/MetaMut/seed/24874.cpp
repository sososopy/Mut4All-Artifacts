
#include <string>
#include <initializer_list>

void __attribute__((noipa))
tata(std::initializer_list<std::string> init)
{
  throw 1;
}

int
main()
{
  try
    {
      tata({ "0123456789012346" }); // using shorter string or "..."s works
    }
  catch (...)
    {
    }
}


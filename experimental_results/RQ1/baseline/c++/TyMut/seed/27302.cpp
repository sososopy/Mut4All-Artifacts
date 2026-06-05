
#include <string>

void foo (const std::string& s)
{
  struct GCCLibSuffix {
    std::string LibSuffix;
    bool Active;
  }
  Suffixes[] =
    {
     {"/gcc/" + s, true}
    };
}

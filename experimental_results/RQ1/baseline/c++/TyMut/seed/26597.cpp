
#include <optional>
#include <utility>

const int &optional_arg_or(std::optional<int> &def,
			   int &&def_val)
{
  def = def_val;

  return *def;
}


int gimme()
{
  std::optional<int> def;

  const int &v=optional_arg_or(def, 0);

  int bologna=v;
  
  return bologna;
}



export module hello;

import <string_view>;

export namespace hello
{
  inline bool check (const std::string_view& n)
  {
    return !n.empty ();
  }
}


import hello;

int main ()
{
  return hello::check ("World") ? 0 : 1;
}

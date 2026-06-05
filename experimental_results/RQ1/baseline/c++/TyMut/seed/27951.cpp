
#include <string>

// allow to steal _text from rvalues
struct Test
{
  std::string const& str() const& { return _text; }
  std::string &&     str() &&     { return std::move(_text); }

  operator std::string const&() const& { return _text; }
  operator std::string &&    () &&     { return std::move(_text); }

  std::string _text;
};

int main()
{
  Test t;
  std::string a { std::move(t).str() };   // 1
  std::string b { std::move(t) };         // 2
  return 0;
}

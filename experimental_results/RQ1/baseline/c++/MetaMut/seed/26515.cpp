
#include <map>
#include <string>

class T
{
  static inline std::map<std::string, int> const m = {{"foo", 0}};

public:
  static int const &Get(std::string const &s) { return m.at(s); }
};

int main()
{
  [[maybe_unused]] int const &i = T::Get("foo");
}


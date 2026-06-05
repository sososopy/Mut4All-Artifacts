
template<typename _CharT>
class basic_string_view
{
public:
  basic_string_view(const _CharT* __str) noexcept
  {}
  bool
    empty() const noexcept
  { return !_M_len; }
  
private:
  unsigned _M_len;
};

using string_view = basic_string_view<char>;

// bug_b.ii
export module hello;

import "bug_a.ii";

export inline bool Check (const string_view& n)
{
  return !n.empty ();
}


// bug_c.ii
import hello;

int main ()
{
  return Check ("World") ? 0 : 1;
}


// bug_a.ii
template<typename _T1>
struct pair
{
  pair() { }
};

inline pair<bool> blob ()
{
  return {};
}

// bug_b.ii
import  "bug_a.ii";

pair<bool> boom();


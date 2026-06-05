
template <typename _TAny, int value = _TAny::param>
struct s{};

template <typename _TAny>
struct s<_TAny, _TAny::param>
{
  typedef int type;
};
struct checked_type
{
    static const int param = 0;
};

s<checked_type>::type  t;

int main()
{
    return t;
}

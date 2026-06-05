
namespace t
{
   class g{}cout;
   template<class ...T>
   struct v
   {
     template <class T1> v(T1&&){}
   };
  class monostate {};
  g &operator <<(g &out, const char *a);
}
struct V
{
    int a;
};
inline t::g& operator << (t::g& os, const t::v<t::monostate, V>& a)
{
    V v;
    os << v;
    return os;
}

int main()
{
    V v={4};
    t::cout << v;
    return 0;
}


struct ostream {
  ostream& operator<<(int);
};

struct sfinae_base {

  typedef char one;
  typedef char (&two)[2];

  template<class T>
  static T make();

  template<unsigned> struct ok { typedef int type; };

  template<class U, class T>
  static one test(typename ok<sizeof(
    make<U>() << make<T>()
  )>::type);

  template<class, class>
  static two test(...);

};

template<class T>
struct is_printable : private sfinae_base
{
  enum { value = sizeof(test<ostream&, T>(0)) == sizeof(one) };
};

typedef int ok[is_printable<int>::value ? 1 : -1];

int main() {}

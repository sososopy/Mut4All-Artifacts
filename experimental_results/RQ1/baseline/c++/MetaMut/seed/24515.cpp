
template <class T>
struct Y
{
  template <class U> static U x;
};

template <>
template <>
int Y<float>::x<int> = 42; // { dg-bogus "non-member-template declaration" }

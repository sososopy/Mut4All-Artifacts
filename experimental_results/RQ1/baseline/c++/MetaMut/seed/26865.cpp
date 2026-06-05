
class A
{
};
template <typename _Tp>
void
make_shared ()
{
  _Tp ();
}

class B : virtual A
{
public:
  template <typename...> B () {}
};
auto f = make_shared<B>;

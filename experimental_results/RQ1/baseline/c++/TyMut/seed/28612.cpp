
struct Foo {};
struct Bar {};

constexpr int Baz(const Foo&) { return 0; }
constexpr int Baz(Bar&&) { return 1; }

int main()
{
  static_assert( Baz({}) == 1 );
}

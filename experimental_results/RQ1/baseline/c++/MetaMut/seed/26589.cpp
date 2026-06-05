
struct T {
  const char *d = init ();
  constexpr int size () const { return 2; }
  constexpr const char *data () const { return d; }
  constexpr const char *init () const { return new char[2] { 'o', 'k' }; }
  constexpr ~T () { delete[] d; }
};
constexpr int a = T{}.size (); // Ok, a = 2
constexpr int b = T{}.data ()[0]; // Ok, b = 'o'
constexpr const char *c = T{}.data (); // Not constant expression, because it returns
// address from new which is later in the dtor deleted.
static_assert (false, T{}); // Valid?

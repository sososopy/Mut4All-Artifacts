
template <typename T> struct Inner {
  static int member;
};
void test() {
 struct local {};
 Inner<local>::member;
}
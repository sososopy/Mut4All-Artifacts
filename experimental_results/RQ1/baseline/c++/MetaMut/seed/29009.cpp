
template<class T> void f() {
  []<class>() {

  }.operator()<T>();
}
int main() {
  f<int>();
}

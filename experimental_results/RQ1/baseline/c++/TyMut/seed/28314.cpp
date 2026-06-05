
class A { ~A(); };
template <class> class B { A f = 1; };
int main() {
  new B<int>;
}

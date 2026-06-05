
template <typename T>
struct C  {
  virtual void f()  {
    T().waldo();
  }
};
int main() {
  C<int> obj;
}

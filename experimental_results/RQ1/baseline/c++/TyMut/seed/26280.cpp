# 1 "a.cpp" 1
template <class T>
struct foo {
  T element;
};

template <class T>
class a {
  a();
};

template <class T>
a<T>::a() {
  struct foo<T> *temp = new struct foo;
}

int main() {

  a<int> object;

  return 0;
}


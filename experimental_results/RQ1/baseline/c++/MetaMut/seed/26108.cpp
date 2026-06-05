template <class T1, class T2>
void function(T1& value, T2& object) {
  value = object.method();
}

template <class T1>
void function_broken(T1& value) {
  struct {
    T1 method() {
      return v;
    }
    T1 v;
  } object;

  function(value, object);
}

int main() {
  int value;
  function_broken(value);

  return 0;
}

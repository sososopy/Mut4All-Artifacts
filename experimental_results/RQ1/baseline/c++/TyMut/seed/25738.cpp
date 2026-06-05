
template <typename T>
struct ResultCallback {
  virtual void Run(T) = 0;
};

template <typename T>
struct FunctionResultCallback: public ResultCallback<T> {
  void Run(T a) { }
};

template <typename T>
ResultCallback<T>*
NewCallback(void (*function)(T))
{
  return new FunctionResultCallback<T>;
}

int main(int argc, char** argv) {
  struct Bar {
    static void Do(Bar*) { }
  };
  NewCallback(&Bar::Do)->Run(0);
}


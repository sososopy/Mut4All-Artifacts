
template <typename T>
void TemplateFunctionWithInnerClass() {
  class InnerNotAssignable {
  public:
    InnerNotAssignable &operator=(InnerNotAssignable &&Other) = default;
  private:
    const T TVar{};
  };
};

void Foo() {
  TemplateFunctionWithInnerClass<int>();
}


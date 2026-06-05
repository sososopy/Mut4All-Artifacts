struct TestClass {
  void testFinction() {}
  static TestClass* ptr_;
  constexpr static auto funcPtr = +[](){ ptr_->testFinction(); };
};

TestClass* TestClass::ptr_ = new TestClass();

int main() {
  TestClass::funcPtr();
  delete TestClass::ptr_;
  return 0;
}
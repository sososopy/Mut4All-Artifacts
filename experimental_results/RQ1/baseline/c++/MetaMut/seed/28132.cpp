
extern "C" void* malloc(decltype(sizeof(0)));
extern "C" void free(void*);

struct string {
  string(int) : s(malloc(1)) { }
  ~string() { free(s); }
  string(const string& str) : s(str.s ? malloc(1) : nullptr) { }
  void* s;
  bool empty() const { return s == nullptr; }
};
    
string foo()
{
  string s(42);
  return s.empty() ? throw "empty" : s;
}

int main()
{
  foo();
}

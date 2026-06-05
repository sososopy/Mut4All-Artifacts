

class foo {
  ~foo() {}
};

int main() { 
  new foo;   // OK
  new foo(); // OK
  new foo{}; // error: 'foo::~foo()' is private
}


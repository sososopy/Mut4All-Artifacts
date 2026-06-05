struct S { 
  int i = 42;
  constexpr auto f() { 
    return [this](this auto) { 
      return this->i;
    }(); 
  }; 
};

int main() {
  return S().f();
}


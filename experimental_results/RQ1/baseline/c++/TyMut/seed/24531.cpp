
    int* foo(){ return nullptr; }
    int& bar(){ return foo() != nullptr ? *foo() : throw 0; } // error
 
    int main() {
      int& ref = bar();
    }


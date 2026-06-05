
    
    template<typename T>
    constexpr T foo { 1.2345 };
        
    template<typename T>
    T fun(T x) {
      return -foo<T> * x;
    }
        
    int main() {
      fun(2.0);
    }
    

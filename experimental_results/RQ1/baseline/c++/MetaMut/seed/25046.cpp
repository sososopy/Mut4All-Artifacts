

    template<typename T, typename U>
    struct is_same {
        constexpr operator bool() const noexcept {
            return false;
        }
    };
    template<typename T>
    struct is_same<T, T> {
        constexpr operator bool() const noexcept {
            return true;
        }
    };
    
    template<typename Z>
    void foo(Z){
        // This works with clang++ but does not with g++.
        constexpr bool b1 = is_same<int, Z>{};
        
        // This works with both.
        constexpr bool b2 = is_same<int, Z>();
    }
    
    int main(){
        foo(1);
        // This works with both, too.
        
        constexpr bool b3 = is_same<int, int>();
    }

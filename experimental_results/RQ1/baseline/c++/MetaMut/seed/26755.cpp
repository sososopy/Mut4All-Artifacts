
    #include <type_traits>
    
    struct T;
    
    template <typename, typename = void>
    struct F { };
    
    template <typename X>
    struct F<X, std::enable_if_t<std::is_same<X, T>{}()>> {
        template <typename MakeDependent>
        F(MakeDependent) {
            auto ICE_HERE = __func__;
            (void)ICE_HERE; // avoid -Wunused-variable
        }
    };
    
    int main() {
        F<T>{1};
    }

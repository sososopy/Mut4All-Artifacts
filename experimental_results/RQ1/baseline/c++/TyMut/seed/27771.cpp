
template< typename T >
struct G {

    using my_int = char;
    static constexpr my_int my_array[1] {0};

};

void work() {
    G<double>::my_array[0];
}

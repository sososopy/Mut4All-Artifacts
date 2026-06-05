
struct tensor {
    constexpr tensor( int value ) : _value( value ) {}
    template <int... Indices> constexpr int value() const { return _value; }
private:
    int _value;
};

template <typename T> struct printer {
    static int get( T const & tensor ) { return detail<>::get( tensor ); }
private:
    template <int... Indices> struct detail {
        static int get( T const & tensor ) { return tensor.value<Indices...>(); }
    };
};

int main() {
    printer<tensor>::get( tensor<int>(1) );
}

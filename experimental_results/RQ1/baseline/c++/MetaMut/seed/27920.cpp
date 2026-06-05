
template <typename T>
struct Struct {
    void operator()( int x, int y ) { }
    void foo() {
        [this]( auto x ){ operator()( x, x ); }( 42 );
    }
};

int main()
{
    Struct<int> x;
    x.foo();
    return 0;
}

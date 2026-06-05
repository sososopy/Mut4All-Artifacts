
template< typename T >
class B
{
    public:
        void Foo() {}

        void Bar()
        {
            [&]()
            {
                [&]() { Foo(); }();
            }();
        }
};

int main( int argc, char** argv )
{
    B<int> b;
    b.Bar();
    return 0;
}


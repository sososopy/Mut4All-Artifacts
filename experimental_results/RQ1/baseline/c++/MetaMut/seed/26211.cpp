
struct B
{
        template<int N>
        struct A
        {
                constexpr int get_N() { return N; }
 
//              int X = N;
                int X = get_N();
                void foo( )
                {
                        int x = N;
                }
        };
};

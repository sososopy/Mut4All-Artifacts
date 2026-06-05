
template <int N>
struct A { static const int X = N; };

#define ASSERT(e) static_assert (e, #e)

template <class T, int N>
struct B: A<N>
{
    using A<N>::X;

    static void foo ()
    {
        char a __attribute__ ((vector_size (N)));
        ASSERT (sizeof a == N);

        T b __attribute__ ((vector_size (N)));
        ASSERT (sizeof b == N);
    }

    static void bar ()
    {
        char c __attribute__ ((vector_size (X)));
        ASSERT (sizeof c == X);

        T d __attribute__ ((vector_size (X)));
        ASSERT (sizeof d == X);
    }
};

void bar ()
{
    B<int, 16>::foo ();
    B<int, 16>::bar ();
}



#include <cstdint>
#include <tuple>
#include <iostream>

using std::size_t;

template<size_t N>
struct string_constexpr
{   
    constexpr string_constexpr() = default;

    template<size_t M>
    constexpr string_constexpr( char const (&d)[M] ):
        data{0}
    {
        static_assert( M <= N, "size!" );
		for(size_t i=0; i != M; i++)
		{
			data[i] = d[i];
		}
    }
    
	char data[N];
};

template<typename T, size_t N> 
T& operator<<(T& stream, string_constexpr<N> const& str)
{
    return (stream << str.data);
}

template<int N>
constexpr string_constexpr<N> s( char const (&d)[N] )
{
    string_constexpr<N> c{};
	for(size_t i=0; i != N; i++)
	{
		c.data[i] = d[i];
	}
    
    return c;
}

template<size_t N, size_t M>
constexpr auto concat(string_constexpr<N> const& s1, string_constexpr<M> const& s2)
{
	string_constexpr<N+M-1> s( s1.data );
	
	for(size_t i=0; i != M; i++)
	{
		s.data[N+i-1] = s2.data[i];
	}
	
	return s;
}

template<size_t N, size_t M>
constexpr auto concat(char const (&x)[N], char const (&y)[M])
{
    static_assert(N == 4 && M == 4, "");
	string_constexpr<N+M-1> tmp{x};

	for(size_t i=0; i != M; i++)
	{
		tmp.data[N+i-1] = y[i];
	}
	
	return tmp;
}


void foo()
{
   auto constexpr s1 = s( "bla" );
   auto constexpr s2 = s( "blub" );
   
   string_constexpr<8> constexpr s1s2 = concat(s1,s2);
   auto constexpr c = concat("bla", "blub");
   std::cout << s1.data << std::endl << s2.data << std::endl << s1s2.data << std::endl << c << std::endl;
}

int main()
{
foo();
return 0;
}


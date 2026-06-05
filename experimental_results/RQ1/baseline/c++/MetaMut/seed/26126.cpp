

#include <iostream>

using namespace std;

template<class E, E V, int CNT>
struct seq_t
{
     template<E... Es> struct seq{};

     template<int N, E... Es>
     struct gen : gen<N - 1, V, Es...>{};

     template<E... Es>
     struct gen<0, Es...> : seq<Es...>{};

     struct bits_t{ E e[CNT]; };

     template<E... Es>
     static constexpr bits_t init(seq<Es...>) {return {{Es...}};}

     static constexpr bits_t init() {return init(gen<CNT>{});}
};



int main()
{
     typedef seq_t<int, 123, 5> wow;

     constexpr wow::bits_t bits(wow::init());

     cout << bits.e[0] << endl;
     cout << bits.e[1] << endl;
     cout << bits.e[2] << endl;
     cout << bits.e[3] << endl;
     cout << bits.e[4] << endl;
}

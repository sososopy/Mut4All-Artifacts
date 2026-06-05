
template<int... values>
static void Colors()
{
    static const int colors[] = { values... };
    // ^ This version passes in G++ 4.6 and Clang++ 3.0, fails in G++ 4.7 and 4.8

    //static const int colors[sizeof...(values)] = { values... };
    // ^This passes in all of them

    for(auto c: colors) { }
    // ^ This line is the one that gets the error message
}

int main()
{
    Colors<0,1,2> ();
}

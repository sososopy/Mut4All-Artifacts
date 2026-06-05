
# include <cctype>
# include <algorithm>

template<int (& F)(int)>
constexpr int safeCtype(unsigned char c) noexcept(noexcept(F(c)))
{ return F(c); }

int main()
{
    const char t[] = "a";
    //safeCtype<std::isspace>(*t);
    std::find_if(t, t + 1, safeCtype<std::isspace>);
}


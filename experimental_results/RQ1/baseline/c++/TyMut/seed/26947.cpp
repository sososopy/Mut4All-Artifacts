
template<bool Head, bool... Tail>
constexpr bool any = Head || any<Tail...>;

template<bool Head>
constexpr bool any<Head> = Head;

int main()
{
    static_assert(any<false, false, true, false>, "");
}


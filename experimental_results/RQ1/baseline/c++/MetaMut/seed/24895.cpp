
int main() {
    constexpr auto func = []()constexpr { return 1; };
    auto i = []{return func();};
}   



struct arr {
    constexpr arr() : elem() {}
    char elem[17];
};

constexpr arr f()
{
    arr result;
    return result;
}

constexpr arr a{f()};

int main(){}

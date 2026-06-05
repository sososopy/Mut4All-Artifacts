template<typename T>
struct Spamee{
    static_assert(T::num != -1);
};
template<long I>
struct Spammer{
    static constexpr int num = I;
    Spamee<Spammer<I>> val{};
    Spammer<I-1> s;
};
template<>
struct Spammer<0>{
    static constexpr int num = 0;
    Spamee<Spammer<0>> val{};
};

int main(){
    auto s = Spammer<1000>{};
}
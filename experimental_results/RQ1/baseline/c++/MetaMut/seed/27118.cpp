template<typename T>
class number {
    T num;
public:
    number(T num = 0): num(num) {}
    
    friend auto add(auto a, auto b);
};

auto add(auto a, auto b) {
    // unjustified compilation error on the line below
    return number{a}.num + number{b}.num;
}

int main() {
    auto result = add(1.0, 2.0);
}


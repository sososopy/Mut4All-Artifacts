
#include <string>
#include <cstring>
#include <array>
#include <vector>
#include <utility>

class String {
public:
    template<size_t n>String(const char(&s)[n])noexcept:value(){
        if constexpr(n<=16){
            std::memcpy(value.data(),s,n);}
        else{value=std::array<unsigned char,16>{};}}
    template<typename... Args, std::enable_if_t<std::conjunction_v<std::is_constructible<std::string, Args>...>, int> = 0>
    explicit String(Args&&... args) : value(std::forward<Args>(args)...) {}
private:
    std::array<unsigned char,16>value;
};

int main() {
    auto check=[&](std::vector<std::pair<String,String>>textInputs){};
    check({{{"Hello", "World"}, {"Foo", "Bar"}}});
    return 0;
}


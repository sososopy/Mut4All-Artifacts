
#include <stdexcept>
#include <cassert>

int num = 0;
struct ptr{
    ptr(){
        ++num;
    }
    ptr(ptr&&){
        ++num;
    }
    ~ptr(){
        assert(num-- > 0);
    }
};

struct ThrowOnExit{
    ~ThrowOnExit() noexcept(false){
        throw std::runtime_error("");
    }
};

ptr foo(ptr x){
    try{
        ThrowOnExit _;
        return x;
    }catch (const std::exception&) {
        throw;
    }
}

void wrapper(){
    try{
        foo(ptr{});
    }catch(const std::exception&){}
}

int main(){
    wrapper();
}


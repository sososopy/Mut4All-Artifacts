volatile int a = 0;
struct function {
    function& operator=(function const&) {
        a = 1;
        return *this;
    }
};
int main() {
    function list;
    [&list](this auto self) {
        list = function{};
    }();
}
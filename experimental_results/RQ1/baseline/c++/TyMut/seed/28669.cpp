template<typename...T>
struct A:T...{
    using T::T...;
};
int main() {
}

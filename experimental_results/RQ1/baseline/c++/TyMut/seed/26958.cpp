
struct Value
{
    template<class T>
    static constexpr T value = 0;
};

template<typename TValue>
struct Something
{
    void foo() {
        static_assert(TValue::template value<int> == 0, ""); // error
    }
};

int main() { 
    Something<Value>{}.foo();
}


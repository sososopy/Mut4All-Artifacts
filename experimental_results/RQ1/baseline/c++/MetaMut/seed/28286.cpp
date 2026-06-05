

template <typename IterT, typename FuncT>
void for_each(IterT b, IterT e, FuncT f) {
    while (b != e) {
        f(*b++);
    }
}

volatile int sink; 
void xxx(int x) {
    sink = x;
}

void foo() {
    static constexpr auto THIS_IS_USED = 1;  
    int arr[10]{1,2,3,4,5,6,7,8,9,10};

    for_each(arr, arr+10, [](auto v) {
        xxx(THIS_IS_USED + v);
    });
}


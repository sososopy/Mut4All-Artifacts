
template<class T>
void foo(T) {
    [](auto){
        [] {
            struct X {};
        };
    };
}

template void foo(int);


template<typename T>
class Foo {
public:
    Foo() {
        ++c;
    }
private:
    thread_local static int c;
};

template<typename T> thread_local int Foo<T>::c(0);

int main(){
    Foo<int> o;
}


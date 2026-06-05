
template <class T>
void
foo(T t) {
    [&i = t.cbegin()]() {};
}

int main() {
    return 0;
}
